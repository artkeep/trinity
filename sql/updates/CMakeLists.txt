add_subdirectory(3.3.5a_old)

FILE(GLOB _SQL_UPDATES *.sql)

INSTALL(FILES ${_SQL_UPDATES} DESTINATION share/trinity/sql/updates)
