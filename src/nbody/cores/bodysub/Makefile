# 	$NEMO/src/nemo/nbody/core/bodysub
include $(NEMOLIB)/makedefs

SRC = bti_0.c bti_1.c bti_i.c bti_key.c btr_0.c btr_1.c btr_aux.c \
       btr_ax.c btr_ay.c btr_az.c btr_i.c btr_key.c btr_m.c btr_phi.c \
       btr_r.c btr_t.c btr_v.c btr_vr.c btr_vt.c btr_vp.c btr_vx.c btr_vy.c btr_vz.c \
       btr_x.c btr_y.c btr_z.c btr_etot.c btr_ekin.c btr_jtot.c \
       btr_dens.c btr_eps.c \
       btr_glon.c btr_glat.c btr_mul.c btr_mub.c \
       btr_xsky.c btr_ysky.c

OBJ  = bti_0.so bti_1.so bti_i.so bti_key.so btr_0.so btr_1.so btr_aux.so \
       btr_ax.so btr_ay.so btr_az.so btr_i.so btr_key.so btr_m.so btr_phi.so \
       btr_r.so btr_t.so btr_v.so btr_vr.so btr_vt.so btr_vp.so btr_vx.so btr_vy.so btr_vz.so \
       btr_x.so btr_y.so btr_z.so btr_etot.so btr_ekin.so btr_jtot.so \
       btr_dens.so btr_eps.so \
       btr_glon.so btr_glat.so btr_mul.so btr_mub.so \
       btr_xsky.so btr_ysky.so

INC = bodytrans.h

SRC_DIR = $(NEMO)/src/nbody/core/bodysub

OBJ_DIR = $(NEMOOBJ)/bodytrans

# Defensive
help:
	@echo Bodytrans stuff

# Targets used to install code in Nemo.

install: .install_obj

.install_inc: $(INC)
	cp $(INC) $(NEMOINC)
	@touch .install_inc

.install_obj: $(OBJ)
	cp $? $(OBJ_DIR)

.install_src: Makefile $(SRC) $(INC)
	cp $? $(SRC_DIR)
	@touch .install_src

update:
	@echo No update here yet: use install

# Targets used to update Nemo.

nemo_lib: nemo_ann $(OBJ)
	mv $(OBJ) $(OBJ_DIR)
	cp BTclean $(OBJ_DIR)

nemo_bin: nemo_ann

bins:

nemo_ann:
	@echo "DIR: `pwd`"

nemo_src:

clean:
	rm -f *.o *.so *.o~ *.a core

$(OBJ): $(SRC)

# new rule to go from .c to .o if LDOPEN is used and sharable .so must
# be created:
#
#.c.so:
#	$(CC) $(CFLAGS) $(LOCAL_INC) -c $<
#	ldso $*

