#!/bin/bash
## running ALL producers 
#run producer ALUMINIUM
gnome-terminal -- ./producer ALUMINIUM 600 0.6 2000 40
#run producer COPPER
gnome-terminal -- ./producer COPPER 214 0.20 3200 40
#run producer COTTON
gnome-terminal -- ./producer COTTON 123 0.30 2000 40
#run producer CRUDEOIL
gnome-terminal -- ./producer CRUDEOIL 20 0.9 2000 40
#run producer GOLD
gnome-terminal -- ./producer GOLD 180 0.23 1000 40
#run producer LEAD
gnome-terminal -- ./producer LEAD 7.1 0.5 2000 40
#run producer MENTHAOIL
gnome-terminal -- ./producer MENTHAOIL 1230 0.10 1100 40
#run producer NATURALGAS
gnome-terminal -- ./producer NATURALGAS 7.1 0.5 2000 40
#run producer NICKEL
gnome-terminal -- ./producer NICKEL 141 0.5 6000 40
#run producer SLIVER
gnome-terminal -- ./producer SLIVER 32 0.5 8600 40
#run producer ZINC
gnome-terminal -- ./producer ZINC 12 0.5 3200 40

#____________________________________________________________________
#run consumer
gnome-terminal -- ./consumer 40 