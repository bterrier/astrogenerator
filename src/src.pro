TEMPLATE = subdirs
SUBDIRS += \
    astrogenerator \
    genericwidgets \
    libastrogenerator \
    libastrocalc
    
libastrogenerator.depends += libastrocalc
astrogenerator.depends += libastrogenerator
