set(requires i2c_bus)

idf_component_register(
	SRC_DIRS .
    INCLUDE_DIRS .
    REQUIRES ${requires}
)
