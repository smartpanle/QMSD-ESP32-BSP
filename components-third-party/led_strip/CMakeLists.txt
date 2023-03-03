set(requires driver)

idf_component_register(
    SRC_DIRS .
    INCLUDE_DIRS include
    REQUIRES ${requires}
)
