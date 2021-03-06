/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6678 $
 * $Id: coderefactoring.cpp 6678 2010-10-08 05:07:32Z loaden $
 * $HeadURL: http://svn.berlios.de/svnroot/repos/codeblocks/branches/codecompletion_refactoring/src/plugins/codecompletion/coderefactoring.cpp $
 */

#include <sdk.h>
#include "coderefactoring.h"
#include "nativeparser.h"
#include "cbstyledtextctrl.h"
#include "cbproject.h"
#include "encodingdetector.h"
#include "searchresultslog.h"

#include <wx/progdlg.h>

#define CC_CODEREFACTORING_DEBUG_OUTPUT 0

#if CC_CODEREFACTORING_DEBUG_OUTPUT == 1
    #define TRACE(format, args...) \
        Manager::Get()->GetLogManager()->DebugLog(F(format, ##args))
    #define TRACE2(format, args...)
#elif CC_CODEREFACTORING_DEBUG_OUTPUT == 2
    #define TRACE(format, args...)                                              \
        do                                                                      \
        {                                                                       \
            if (g_EnableDebugTrace)                                             \
                Manager::Get()->GetLogManager()->DebugLog(F(format, ##args));   \
        }                                                                       \
        while (false)
    #define TRACE2(format, args...) \
        Manager::Get()->GetLogManager()->DebugLog(F(format, ##args))
#else
    #define TRACE(format, args...)
    #define TRACE2(format, args...)
#endif

CodeRefactoring::CodeRefactoring(NativeParser& np) :
    m_NativeParser(np)
{
}

CodeRefactoring::~CodeRefactoring()
{
}

wxString CodeRefactoring::GetSymbolUnderCursor()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return wxEmptyString;

    cbStyledTextCtrl* control = editor->GetControl();
    const int style = control->GetStyleAt(control->GetCurrentPos());
    if (control->IsString(style) || control->IsComment(style))
        return wxEmptyString;

    if (!m_NativeParser.GetParser().Done())
    {
        cbMessageBox(_("C++ Parser is still parsing files..."), _("Code Refactoring"), wxOK | wxICON_WARNING);
        return wxEmptyString;
    }

    const int pos = editor->GetControl()->GetCurrentPos();
    const int start = editor->GetControl()->WordStartPosition(pos, true);
    const int end = editor->GetControl()->WordEndPosition(pos, true);
    return editor->GetControl()->GetTextRange(start, end);
}

bool CodeRefactoring::Parse()
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!editor)
        return false;

    const wxString targetText = GetSymbolUnderCursor();
    if (targetText.IsEmpty())
        return false;

    TokenIdxSet targetResult;
    const int endOfWord = editor->GetControl()->WordEndPosition(editor->GetControl()->GetCurrentPos(), true);
    m_NativeParser.MarkItemsByAI(targetResult, true, false, true, endOfWord);
    if (targetResult.empty())
    {
        cbMessageBox(_("Symbol not found under cursor!"), _("Code Refactoring"), wxOK | wxICON_WARNING);
        return false;
    }

    // handle local variables
    bool isLocalVariable = false;
    Token* token = m_NativeParser.GetParser().GetTokens()->at(*targetResult.begin());
    if (token)
    {
        Token* parent = token->GetParentToken();
        if (parent && parent->m_TokenKind == tkFunction)
            isLocalVariable = true;
    }

    wxArrayString files;
    cbProject* project = m_NativeParser.GetProjectByFilename(editor->GetFilename());

    if (isLocalVariable || !project)
        files.Add(editor->GetFilename());
    else
    {
        const int ret = cbMessageBox(_("Only search open files? Select \"No\" search the project!"),
                                     _("Code Refactoring"), wxICON_QUESTION | wxYES_NO | wxCANCEL);
        if (ret == wxID_YES)
            GetOpenedFiles(files);
        else if (ret == wxID_NO)
            GetAllProjectFiles(files, project);
        else
            return false;
    }

    if (files.IsEmpty())
        return false;

    size_t count = SearchInFiles(files, targetText);
    if (count)
        count = VerifyResult(targetResult, targetText, isLocalVariable);

    return count != 0;
}

void CodeRefactoring::FindReferences()
{
    if (Parse())
        DoFindReferences();
}

void CodeRefactoring::RenameSymbols()
{
    const wxString targetText = GetSymbolUnderCursor();
    if (targetText.IsEmpty())
        return;

    wxString replaceText = wxGetTextFromUser(_("Rename symbols under cursor"),
                                             _("Code Refactoring"),
                                             targetText,
                                             Manager::Get()->GetAppWindow());
    if (!replaceText.IsEmpty() && replaceText != targetText && Parse())
        DoRenameSymbols(targetText, replaceText);
}

size_t CodeRefactoring::SearchInFiles(const wxArrayString& files, const wxString& targetText)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    m_SearchDataMap.clear();

    // now that list is filled, we'll search
    wxWindow* parent = edMan->GetBuiltinActiveEditor()->GetParent();
    cbStyledTextCtrl* control = new cbStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(0, 0));
    control->Show(false);

    // let's create a progress dialog because it might take some time depending on the files count
    wxProgressDialog* progress = new wxProgressDialog(_("Code Refactoring"),
                                                      _("Please wait while searching inside the project..."),
                                                      files.GetCount(),
                                                      Manager::Get()->GetAppWindow(),
                                                      wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);
    PlaceWindow(progress);

    for (size_t i = 0; i < files.GetCount(); ++i)
    {
        // update the progress bar
        if (!progress->Update(i))
            break; // user pressed "Cancel"

        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(files[i]);
        if (ed)
            control->SetText(ed->GetControl()->GetText());
        else // else load the file in the control
        {
            EncodingDetector detector(files[i]);
            if (!detector.IsOK())
                continue; // failed
            control->SetText(detector.GetWxStr());
        }

        Find(control, files[i], targetText);
    }

    delete control; // done with it
    delete progress; // done here too

    return m_SearchDataMap.size();
}

size_t CodeRefactoring::VerifyResult(const TokenIdxSet& targetResult, const wxString& targetText,
                                     bool isLocalVariable)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return 0;

    Token* parentOfLocalVariable = nullptr;
    if (isLocalVariable)
    {
        Token* token = m_NativeParser.GetParser().GetTokens()->at(*targetResult.begin());
        parentOfLocalVariable = token->GetParentToken();
    }

    // now that list is filled, we'll search
    cbStyledTextCtrl* control = new cbStyledTextCtrl(editor->GetParent(), wxID_ANY, wxDefaultPosition,
                                                     wxSize(0, 0));
    control->Show(false);

    // styled the text to support control->GetStyleAt()
    cbEditor::ApplyStyles(control);
    EditorColourSet edColSet;

    size_t totalCount = 0;
    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
        totalCount += it->second.size();

    // let's create a progress dialog because it might take some time depending on the files count
    wxProgressDialog* progress = new wxProgressDialog(_("Code Refactoring"),
                                                      _("Please wait while verifying result..."),
                                                      totalCount,
                                                      Manager::Get()->GetAppWindow(),
                                                      wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);
    PlaceWindow(progress);

    size_t task = totalCount;
    TokenIdxSet result;
    bool userBreak = false;
    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end();)
    {
        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(it->first);
        if (ed)
            control->SetText(ed->GetControl()->GetText());
        else // else load the file in the control
        {
            EncodingDetector detector(it->first);
            if (!detector.IsOK())
            {
                task -= it->second.size();
                m_SearchDataMap.erase(it++);
                continue; // failed
            }
            control->SetText(detector.GetWxStr());
        }

        // apply the corlor setting
        edColSet.Apply(editor->GetLanguage(), control);

        ccSearchData searchData = { control, it->first };
        for (SearchDataList::iterator itList = it->second.begin(); itList != it->second.end();)
        {
            // update the progress bar
            if (!progress->Update(totalCount - (--task)))
            {
                userBreak = true;
                break; // user pressed "Cancel"
            }

            // skip string or comment
            const int style = control->GetStyleAt(itList->pos);
            if (control->IsString(style) || control->IsComment(style))
            {
                it->second.erase(itList++);
                continue;
            }

            // do cc search
            const int endOfWord = itList->pos + targetText.Len();
            control->GotoPos(endOfWord); // TODO (Loaden) why need goto the pos too?
            m_NativeParser.MarkItemsByAI(&searchData, result, true, false, true, endOfWord);

            // verify result
            TokenIdxSet::iterator findIter = targetResult.begin();
            for (; findIter != targetResult.end(); ++findIter)
            {
                if (result.find(*findIter) != result.end())
                    break;
            }

            if (findIter == targetResult.end()) // not found
                it->second.erase(itList++);
            else
            {
                // handle for local variable
                if (isLocalVariable)
                {
                    Token* token = m_NativeParser.GetParser().GetTokens()->at(*findIter);
                    if (token)
                    {
                        Token* parent = token->GetParentToken();
                        if (parent != parentOfLocalVariable)
                        {
                            it->second.erase(itList++);
                            continue;
                        }
                    }
                }

                ++itList;
            }
        }

        if (it->second.empty())
            m_SearchDataMap.erase(it++);
        else
            ++it;

        if (userBreak)
            break;
    }

    delete control; // done with it
    delete progress; // done here too

    return m_SearchDataMap.size();
}

void CodeRefactoring::Find(cbStyledTextCtrl* control, const wxString& file, const wxString& target)
{
    const int end = control->GetLength();
    int start = 0;
    int pos = 0;

    for (;;)
    {
        int lengthFound;
        pos = control->FindText(start, end, target, wxSCI_FIND_WHOLEWORD | wxSCI_FIND_MATCHCASE, &lengthFound);
        if (pos != wxSCI_INVALID_POSITION)
        {
            start = pos + lengthFound;
            const int line = control->LineFromPosition(pos);
            wxString text = control->GetLine(line).Trim(true).Trim(false);
            m_SearchDataMap[file].push_back(crSearchData(pos, line, text));
        }
        else
            break;
    }
}

void CodeRefactoring::DoFindReferences()
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!editor)
        return;

    SearchResultsLog* searchLog = Manager::Get()->GetEditorManager()->GetSearchResultLogger();

    const wxString focusFile = editor->GetFilename();
    const int focusLine = editor->GetControl()->GetCurrentLine();
    wxFileName fn(focusFile);
    const wxString basePath(fn.GetPath());
    size_t index = 0;
    size_t focusIndex = 0;

    searchLog->Clear();
    searchLog->SetBasePath(basePath);

    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
    {
        for (SearchDataList::iterator itList = it->second.begin(); itList != it->second.end(); ++itList)
        {
            if (it->first == focusFile && itList->line == focusLine)
                focusIndex = index;

            wxArrayString values;
            wxFileName curFn(it->first);
            curFn.MakeRelativeTo(basePath);
            values.Add(curFn.GetFullPath());
            values.Add(wxString::Format(_T("%d"), itList->line + 1));
            values.Add(itList->text);
            searchLog->Append(values, Logger::info);

            ++index;
        }
    }

    if (Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_show_search"), true))
    {
        CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, searchLog);
        CodeBlocksLogEvent evtShow(cbEVT_SHOW_LOG_MANAGER);
        Manager::Get()->ProcessEvent(evtSwitch);
        Manager::Get()->ProcessEvent(evtShow);
    }

    searchLog->FocusEntry(focusIndex);
}

void CodeRefactoring::DoRenameSymbols(const wxString& targetText, const wxString& replaceText)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor)
        return;

    cbProject* project = m_NativeParser.GetProjectByFilename(editor->GetFilename());

    for (SearchDataMap::iterator it = m_SearchDataMap.begin(); it != m_SearchDataMap.end(); ++it)
    {
        // check if the file is already opened in built-in editor and do search in it
        cbEditor* ed = edMan->IsBuiltinOpen(it->first);
        if (!ed)
        {
            ProjectFile* pf = project ? project->GetFileByFilename(it->first) : 0;
            ed = edMan->Open(it->first, it->second.front().pos, pf);
        }

        cbStyledTextCtrl* control = ed->GetControl();
        control->BeginUndoAction();

        for (SearchDataList::reverse_iterator rIter = it->second.rbegin(); rIter != it->second.rend(); ++rIter)
        {
            control->SetTargetStart(rIter->pos);
            control->SetTargetEnd(rIter->pos + targetText.Len());
            control->ReplaceTarget(replaceText);
        }

        control->EndUndoAction();
    }
}

void CodeRefactoring::GetAllProjectFiles(wxArrayString& files, cbProject* project)
{
    if (!project)
        return;

    // fill the search list with all the project files
    for (int i = 0; i < project->GetFilesCount(); ++i)
    {
        ProjectFile* pf = project->GetFile(i);
        if (!pf)
            continue;
        FileType ft = CCFileTypeOf(pf->relativeFilename);
        if (ft != ftOther)
            files.Add(pf->file.GetFullPath());
    }
}

void CodeRefactoring::GetOpenedFiles(wxArrayString& files)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if (edMan)
    {
        for (int i = 0; i < edMan->GetEditorsCount(); ++i)
            files.Add(edMan->GetEditor(i)->GetFilename());
    }
}
