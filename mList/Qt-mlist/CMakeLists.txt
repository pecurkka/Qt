
set (SOURCES
	application.cpp 
	artikel.cpp 
	cache.cpp 
	feedback.cpp 
	konsum.cpp 
	main.cpp 	
	mainwindow.cpp 
	person.cpp
)


set (HEADERS
	application_global.h 
	application.h 
	artikel.h 
	cache.h 
	feedback.h 
	konsum.h 
	mainwindow.h 
	person.h
)

set (FORMS
	mainwindow.ui
)

set (RESOURCES

)


link_directories(/usr/local/lib)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)





if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mlist
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        ${RESOURCES}
    )
else()
    add_executable(mlist
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        ${RESOURCES}
    )
endif()

set_target_properties(mlist PROPERTIES OUTPUT_NAME mlist)


target_link_libraries(mlist 
                        PUBLIC Qt${QT_VERSION_MAJOR}::Core
                        PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
                        PUBLIC Qt${QT_VERSION_MAJOR}::Gui
                        PUBLIC hbocore
                        PUBLIC hbodaemon
                        PUBLIC hbolog
                        PUBLIC hbodbcore
                        PUBLIC hbooptions
                        PUBLIC hboapplication
                        PUBLIC hbosettings
                        PUBLIC hbomessageservice
                        PUBLIC hbomessageclient
                        PUBLIC hbolockinfo
                        PUBLIC hbi
                        PUBLIC bowsearch
                        PUBLIC bohelpers
                        PUBLIC bomitarbeiter
                        PUBLIC boicons
                        PUBLIC smtpclient
)


if(QT_VERSION_MAJOR EQUAL 6)
	target_link_libraries(mlist 
			PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat
	)
    qt_finalize_executable(mlist)
endif()
