bin_PROGRAMS = cb_share_config

INCLUDES = $(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/include/tinyxml

cb_share_config_LDFLAGS = 

cb_share_config_LDADD =  -L../../base/tinyxml -ltinyxml \
			$(WX_LIBS)

cb_share_config_SOURCES = app.cpp \
			mainframe.cpp

noinst_HEADERS = app.h \
			mainframe.h

man_MANS = cb_share_config.1

EXTRA_DIST = cb_share_config.cbp \
		cb_share_config-unix.cbp \
		$(srcdir)/wxsmith/*.wxs \
		$(man_MANS)
