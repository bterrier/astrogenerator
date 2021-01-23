TEMPLATE = subdirs
SUBDIRS += \
    astrogenerator \
    libastrogenerator \
    libastrocalc
    
libastrogenerator.depends += libastrocalc
astrogenerator.depends += libastrogenerator
