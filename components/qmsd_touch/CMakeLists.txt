set(requires driver i2c_bus qmsd_utils)

idf_component_register(
	SRC_DIRS . ft5x06 gt2863 cst816t gt911 chsc6540 sp2010 cst328 cst3240
	INCLUDE_DIRS . ft5x06
    REQUIRES ${requires} 
)
