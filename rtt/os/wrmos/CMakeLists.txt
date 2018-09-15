IF (OROCOS_TARGET STREQUAL "wrmos")
  FILE( GLOB CPPS [^.]*.cpp [^.]*.c)
  FILE( GLOB HPPS [^.]*.hpp [^.]*.h [^.]*.inl)

  GLOBAL_ADD_INCLUDE( rtt/os/wrmos ${HPPS})
  GLOBAL_ADD_SRC( ${CPPS})
ENDIF (OROCOS_TARGET STREQUAL "wrmos")
