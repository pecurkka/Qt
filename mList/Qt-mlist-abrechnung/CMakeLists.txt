set (SOURCES
	abrechnung.cpp 
	application.cpp 
	artikel.cpp 
	cache.cpp 
	konsum.cpp 	
	main.cpp 
	mlistabrechnung.cpp 
	person.cpp
)

set (HEADERS
	abrechnung.h 
	application_globals.h 
	application.h 
	artikel.h 
	cache.h 
	konsum.h 
	mlistabrechnung.h 
	person.h
)

set (FORMS
)

set (RESOURCES

)


link_directories(/usr/local/lib)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)





if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mlistabrechnung
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        ${RESOURCES}
    )
else()
    add_executable(mlistabrechnung
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        ${RESOURCES}
    )
endif()

set_target_properties(mlistabrechnung PROPERTIES OUTPUT_NAME mlist_abrechnung)


target_link_libraries(mlistabrechnung 
                        PUBLIC Qt${QT_VERSION_MAJOR}::Core
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
                        PUBLIC bohelpers
                        PUBLIC bomitarbeiter
                        PUBLIC smtpclient
)


if(QT_VERSION_MAJOR EQUAL 6)
	target_link_libraries(mlistabrechnung 
			PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat
	)
    qt_finalize_executable(mlistabrechnung)
endif()
