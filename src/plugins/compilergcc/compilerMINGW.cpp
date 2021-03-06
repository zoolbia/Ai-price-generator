/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>
#include <prep.h>
#include "compilerMINGW.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include "manager.h"
#include "macrosmanager.h"
#include "logmanager.h"
#include "compilerMINGWgenerator.h"

#include <configmanager.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerMINGW::CompilerMINGW(const wxString& name, const wxString& ID)
    : Compiler(name, ID)
{
    Reset();
}

CompilerMINGW::~CompilerMINGW()
{
    //dtor
}

Compiler * CompilerMINGW::CreateCopy()
{
    Compiler* c = new CompilerMINGW(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

CompilerCommandGenerator* CompilerMINGW::GetCommandGenerator()
{
    return new CompilerMINGWGenerator;
}

void CompilerMINGW::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = _T("mingw32-gcc.exe");
        m_Programs.CPP = _T("mingw32-g++.exe");
        m_Programs.LD = _T("mingw32-g++.exe");
        m_Programs.DBG = _T("gdb.exe");
        m_Programs.LIB = _T("ar.exe");
        m_Programs.WINDRES = _T("windres.exe");
        m_Programs.MAKE = _T("mingw32-make.exe");
    }
    else
    {
        m_Programs.C = _T("gcc");
        m_Programs.CPP = _T("g++");
        m_Programs.LD = _T("g++");
        m_Programs.DBG = _T("gdb");
        m_Programs.LIB = _T("ar");
        m_Programs.WINDRES = _T("");
        m_Programs.MAKE = _T("make");
    }
    m_Switches.includeDirs = _T("-I");
    m_Switches.libDirs = _T("-L");
    m_Switches.linkLibs = _T("-l");
    m_Switches.defines = _T("-D");
    m_Switches.genericSwitch = _T("-");
    m_Switches.objectExtension = _T("o");
    m_Switches.needDependencies = true;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = clogSimple;
    m_Switches.libPrefix = _T("lib");
    m_Switches.libExtension = _T("a");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = false;
    m_Switches.supportsPCH = true;
    m_Switches.PCHExtension = _T("h.gch");
    m_Switches.UseFullSourcePaths = true; // use the GDB workaround !!!!!!!!

    // Summary of GCC options: http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

    m_Options.ClearOptions();
    m_Options.AddOption(_("Produce debugging symbols"),
                _T("-g"),
                _("Debugging"),
                _T(""),
                true,
                _T("-O -O1 -O2 -O3 -Os"),
                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
    wxString gprof_link = _T("-pg");
    if (platform::windows)
        gprof_link = _T("-pg -lgmon");
    m_Options.AddOption(_("Profile code when executed"), _T("-pg"), _("Profiling"), gprof_link);

    wxString category = _("Warnings");

    // warnings
    m_Options.AddOption(_("In C mode, support all ISO C90 programs. In C++ mode, remove GNU extensions that conflict with ISO C++"), _T("-ansi"), category);
    m_Options.AddOption(_("Enable all compiler warnings (overrides many other settings)"), _T("-Wall"), category);
    m_Options.AddOption(_("Enable extra compiler warnings"), _T("-Wextra"), category);
    m_Options.AddOption(_("Stop compiling after first error"), _T("-Wfatal-errors"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), _T("-w"), category);
    m_Options.AddOption(_("Have g++ follow the 1998 ISO C++ language standard"), _T("-std=c++98"), category);
    m_Options.AddOption(_("Have g++ follow the coming C++0x ISO C++ language standard"), _T("-std=c++0x"), category);
    m_Options.AddOption(_("Enable warnings demanded by strict ISO C and ISO C++"), _T("-pedantic"), category);
    m_Options.AddOption(_("Treat as errors the warnings demanded by strict ISO C and ISO C++"), _T("-pedantic-errors"), category);
    m_Options.AddOption(_("Warn if main() is not conformant"), _T("-Wmain"), category);
    m_Options.AddOption(_("Enable Effective-C++ warnings (thanks Scott Meyers)"), _T("-Weffc++"), category);
    m_Options.AddOption(_("Warn whenever a switch statement does not have a default case"), _T("-Wswitch-default"), category);
    m_Options.AddOption(_("Warn whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration"), _T("-Wswitch-enum"), category);
    m_Options.AddOption(_("Warn if a user supplied include directory does not exist"), _T("-Wmissing-include-dirs"), category);
    m_Options.AddOption(_("Warn if a global function is defined without a previous declaration"), _T("-Wmissing-declarations"), category);
    m_Options.AddOption(_("Warn if the compiler detects that code will never be executed"), _T("-Wunreachable-code"), category);
    m_Options.AddOption(_("Warn if a function can not be inlined and it was declared as inline"), _T("-Winline"), category);
    m_Options.AddOption(_("Warn if floating point values are used in equality comparisons"), _T("-Wfloat-equal"), category);
    m_Options.AddOption(_("Warn if an undefined identifier is evaluated in an '#if' directive"), _T("-Wundef"), category);
    m_Options.AddOption(_("Warn whenever a pointer is cast such that the required alignment of the target is increased"), _T("-Wcast-align"), category);
    m_Options.AddOption(_("Warn if anything is declared more than once in the same scope"), _T("-Wredundant-decls"), category);
    m_Options.AddOption(_("Warn about unitialized variables which are initialized with themselves"), _T("-Winit-self"), category);
    m_Options.AddOption(_("Warn whenever a local variable shadows another local variable, parameter or global variable or whenever a built-in function is shadowed"), _T("-Wshadow"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), _T(""), category, _T("-s"), true, _T("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), _T("-O"), category);
    m_Options.AddOption(_("Optimize more (for speed)"), _T("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), _T("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), _T("-O3"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), _T("-Os"), category);
    m_Options.AddOption(_("Expensive optimizations"), _T("-fexpensive-optimizations"), category);
    m_Options.AddOption(_("Don't keep the frame pointer in a register for functions that don't need one"), _T("-fomit-frame-pointer"), category);
    // machine dependent options - cpu arch
    category = _("CPU architecture tuning (choose none, or only one of these)");
    m_Options.AddOption(_("i386"), _T("-march=i386"), category);
    m_Options.AddOption(_("i486"), _T("-march=i486"), category);
    m_Options.AddOption(_("Intel Pentium"), _T("-march=i586"), category);
    m_Options.AddOption(_("Intel Pentium (MMX)"), _T("-march=pentium-mmx"), category);
    m_Options.AddOption(_("Intel Pentium PRO"), _T("-march=i686"), category);
    m_Options.AddOption(_("Intel Pentium 2 (MMX)"), _T("-march=pentium2"), category);
    m_Options.AddOption(_("Intel Pentium 3 (MMX, SSE)"), _T("-march=pentium3"), category);
    m_Options.AddOption(_("Intel Pentium 4 (MMX, SSE, SSE2)"), _T("-march=pentium4"), category);
    m_Options.AddOption(_("Intel Pentium 4 Prescott (MMX, SSE, SSE2, SSE3)"), _T("-march=prescott"), category);
    m_Options.AddOption(_("Intel Pentium 4 Nocona (MMX, SSE, SSE2, SSE3, 64bit extensions)"), _T("-march=nocona"), category);
    m_Options.AddOption(_("Intel Pentium M (MMX, SSE, SSE2)"), _T("-march=pentium-m"), category);
    m_Options.AddOption(_("Intel Core2 (MMX, SSE, SSE2, SSE3, SSSE3, 64bit extensions)"), _T("-march=core2"), category);
    m_Options.AddOption(_("AMD K6 (MMX)"), _T("-march=k6"), category);
    m_Options.AddOption(_("AMD K6-2 (MMX, 3DNow!)"), _T("-march=k6-2"), category);
    m_Options.AddOption(_("AMD K6-3 (MMX, 3DNow!)"), _T("-march=k6-3"), category);
    m_Options.AddOption(_("AMD Athlon (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), _T("-march=athlon"), category);
    m_Options.AddOption(_("AMD Athlon Thunderbird (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), _T("-march=athlon-tbird"), category);
    m_Options.AddOption(_("AMD Athlon 4 (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-4"), category);
    m_Options.AddOption(_("AMD Athlon XP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-xp"), category);
    m_Options.AddOption(_("AMD Athlon MP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-mp"), category);
    m_Options.AddOption(_("AMD K8 core (x86-64 instruction set)"), _T("-march=k8"), category);
    m_Options.AddOption(_("AMD Opteron (x86-64 instruction set)"), _T("-march=opteron"), category);
    m_Options.AddOption(_("AMD Athlon64 (x86-64 instruction set)"), _T("-march=athlon64"), category);
    m_Options.AddOption(_("AMD Athlon-FX (x86-64 instruction set)"), _T("-march=athlon-fx"), category);

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(_T("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(_T("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(_T("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs")));
    if (platform::windows)
    {
        m_Commands[(int)ctLinkNativeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs --subsystem,native")));
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -mwindows")));
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    else
    {
        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(_T("$lib_linker -r -s $static_output $link_objects")));

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
    SetVersionString();
}

void CompilerMINGW::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, _T("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'In function...' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):[ \t]+") + _T("([iI]n ([cC]lass|[cC]onstructor|[dD]estructor|[fF]unction|[mM]ember [fF]unction).*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]+([iI]nstantiated from .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, _T("windres.exe:[ \t](") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, _T("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler note"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Nn]ote:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General note"), cltInfo, _T("([Nn]ote:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):\\(\\.text\\+[0-9a-fA-FxX]+\\):[ \t]([Ww]arning:[ \t].*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + _T("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (3)"), cltError, _T("(") + FilePathWithSpaces + _T("):\\(\\.text\\+[0-9a-fA-FxX]+\\):(.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, _T(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (cannot open output file)"), cltError, _T(".*(ld.*):[ \t](cannot open output file.*):[ \t](.*)"), 2, 1, 0, 3));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, _T("(") + FilePathWithSpaces + _T("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, _T("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Auto-import info"), cltInfo, _T("([Ii]nfo:[ \t].*)\\(auto-import\\)"), 1));
}

AutoDetectResult CompilerMINGW::AutoDetectInstallationDir()
{
    // try to find MinGW in environment variable PATH first
    wxString pathValues;
    wxGetEnv(_T("PATH"), &pathValues);
    if (!pathValues.IsEmpty())
    {
        wxString sep = platform::windows ? _T(";") : _T(":");
        wxChar pathSep = platform::windows ? _T('\\') : _T('/');
        wxArrayString pathArray = GetArrayFromString(pathValues, sep);
        for (size_t i = 0; i < pathArray.GetCount(); ++i)
        {
            if (wxFileExists(pathArray[i] + pathSep + m_Programs.C))
            {
                if (pathArray[i].AfterLast(pathSep).IsSameAs(_T("bin")))
                {
                    m_MasterPath = pathArray[i].BeforeLast(pathSep);
                    return adrDetected;
                }
            }
        }
    }

    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
    {
        // look first if MinGW was installed with Code::Blocks (new in beta6)
        m_MasterPath = ConfigManager::GetExecutableFolder();
        if (!wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
            // if that didn't do it, look under C::B\MinGW, too (new in 08.02)
            m_MasterPath += sep + _T("MinGW");
        if (!wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
        {
            // no... search for MinGW installation dir
            wxString windir = wxGetOSDirectory();
            wxFileConfig ini(_T(""), _T(""), windir + _T("/MinGW.ini"), _T(""), wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
            m_MasterPath = ini.Read(_T("/InstallSettings/InstallPath"), _T("C:\\MinGW"));
            if (!wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
            {
#ifdef __WXMSW__ // for wxRegKey
                // not found...
                // look for dev-cpp installation
                wxRegKey key; // defaults to HKCR
                key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\Dev-C++"));
                if (key.Exists() && key.Open(wxRegKey::Read))
                {
                    // found; read it
                    key.QueryValue(_T("Install_Dir"), m_MasterPath);
                }
                else
                {
                    // installed by inno-setup
                    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Minimalist GNU for Windows 4.1_is1
                    wxString name;
                    long index;
                    key.SetName(_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));
                    //key.SetName("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
                    bool ok = key.GetFirstKey(name, index);
                    while (ok && !name.StartsWith(_T("Minimalist GNU for Windows")))
                    {
                        ok = key.GetNextKey(name, index);
                    }
                    if (ok)
                    {
                        name = key.GetName() + _T("\\") + name;
                        key.SetName(name);
                        if (key.Exists() && key.Open(wxRegKey::Read))
                            key.QueryValue(_T("InstallLocation"), m_MasterPath);
                    }
                }
#endif
            }
        }
        else
            m_Programs.MAKE = _T("make.exe"); // we distribute "make" not "mingw32-make"
    }
    else
        m_MasterPath = _T("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    // don't add lib/include dirs. GCC knows where its files are located

    SetVersionString();
    return ret;
}

void CompilerMINGW::SetVersionString()
{
    /*  NOTE (Biplab#9#): There is a critical bug which blocks C::B from starting up.
        So we'll disable version string checking till we fix the bug. */
    #if !wxCHECK_VERSION(2, 9, 0)
//    Manager::Get()->GetLogManager()->DebugLog(_T("Compiler detection for compiler ID: '") + GetID() + _T("' (parent ID= '") + GetParentID() + _T("')"));

    wxArrayString output, errors;
    wxString sep = wxFileName::GetPathSeparator();
    wxString master_path = m_MasterPath;
    wxString compiler_exe = m_Programs.C;

    /* We should read the master path from the configuration manager as
     * the m_MasterPath is empty if AutoDetectInstallationDir() is not
     * called
     */
    ConfigManager* cmgr = Manager::Get()->GetConfigManager(_T("compiler"));
    if (cmgr)
    {
        wxString settings_path;
        wxString compiler_path;
        /* Differ between user-defined compilers (copies of base compilers) */
        if (GetParentID().IsEmpty())
        {
            settings_path = _T("/sets/")      + GetID() + _T("/master_path");
            compiler_path = _T("/sets/")      + GetID() + _T("/c_compiler");
        }
        else
        {
            settings_path = _T("/user_sets/") + GetID() + _T("/master_path");
            compiler_path = _T("/user_sets/") + GetID() + _T("/c_compiler");
        }
        cmgr->Read(settings_path, &master_path);
        cmgr->Read(compiler_path, &compiler_exe);
    }
    if (master_path.IsEmpty())
    {
        /* Notice: In general this is bad luck as e.g. all copies of a
         * compiler have a different path, most likely.
         * Thus the following might even return a wrong command!
         */
        if (platform::windows)
            master_path = _T("C:\\MinGW");
        else
            master_path = _T("/usr");
    }
    wxString gcc_command = master_path + sep + _T("bin") + sep + compiler_exe;

    Manager::Get()->GetMacrosManager()->ReplaceMacros(gcc_command);
    if (!wxFileExists(gcc_command))
    {
//        Manager::Get()->GetLogManager()->DebugLog(_T("Compiler version detection: Compiler not found: ") + gcc_command);
        return;
    }

//    Manager::Get()->GetLogManager()->DebugLog(_T("Compiler version detection: Issuing command: ") + gcc_command);
    long result = wxExecute(gcc_command + _T(" --version"), output, errors, wxEXEC_NODISABLE);
    if(result != 0)
    {
//        Manager::Get()->GetLogManager()->DebugLog(_T("Compiler version detection: Error executing command."));
    }
    else
    {
        if (output.GetCount() > 0)
        {
//            Manager::Get()->GetLogManager()->DebugLog(_T("Extracting compiler version from: ") + output[0]);
            wxRegEx reg_exp;
            if (reg_exp.Compile(_T("[0-9][.][0-9][.][0-9]")) && reg_exp.Matches(output[0]))
            {
                m_VersionString = reg_exp.GetMatch(output[0]);
//                Manager::Get()->GetLogManager()->DebugLog(_T("Compiler version via RegExp: ") + m_VersionString);
            }
            else
            {
                m_VersionString = output[0].Mid(10);
                m_VersionString = m_VersionString.Left(5);
                m_VersionString.Trim(false);
//                Manager::Get()->GetLogManager()->DebugLog(_T("Compiler version: ") + m_VersionString);
            }
        }
    }
    #endif
}
