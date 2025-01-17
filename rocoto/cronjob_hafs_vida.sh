#!/bin/sh
set -x
date

# NOAA WCOSS Dell Phase3
#HOMEhafs=/gpfs/dell2/emc/modeling/noscrub/${USER}/save/HAFS
#dev="-s sites/wcoss_dell_p3.ent -f"
#PYTHON3=/usrx/local/prod/packages/python/3.6.3/bin/python3

# NOAA WCOSS Cray
#HOMEhafs=/gpfs/hps3/emc/hwrf/noscrub/${USER}/save/HAFS
#dev="-s sites/wcoss_cray.ent -f"
#PYTHON3=/opt/intel/intelpython3/bin/python3

# NOAA RDHPCS Jet
#HOMEhafs=/mnt/lfs4/HFIP/hwrfv3/${USER}/HAFS
#dev="-s sites/xjet.ent -f"
#PYTHON3=/apps/intel/intelpython3/bin/python3

# MSU Orion
 HOMEhafs=/work/noaa/hwrf/save/${USER}/HAFS
 dev="-s sites/orion.ent -f"
 PYTHON3=/apps/intel-2020/intel-2020/intelpython3/bin/python3

# NOAA RDHPCS Hera
#HOMEhafs=/scratch1/NCEPDEV/hwrf/save/${USER}/HAFS
#dev="-s sites/hera.ent -f"
#PYTHON3=/apps/intel/intelpython3/bin/python3

cd ${HOMEhafs}/rocoto

EXPT=$(basename ${HOMEhafs})

#===============================================================================
 # h3db_vidacycst: atm_init+atm_vi+fgat+3denvar+anal_merge and cycling storm
 confh3db_vidacycst="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_vidacycst \
     config.run_atm_init=yes config.run_atm_init_fgat=yes config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=yes config.run_atm_vi_fgat=yes config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=yes config.run_fgat=yes config.run_envar=yes \
     config.gsi_d01=yes config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     vi.vi_storm_env=pert \
     atm_merge.atm_merge_method=vortexreplace analysis_merge.analysis_merge_method=vortexreplace \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3db_vida: atm_init+atm_vi+fgat+3denvar+anal_merge and cycling storm perturbation
 confh3db_vida="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_vida \
     config.run_atm_init=yes config.run_atm_init_fgat=yes config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=yes config.run_atm_vi_fgat=yes config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=yes config.run_fgat=yes config.run_envar=yes \
     config.gsi_d01=yes config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     vi.vi_storm_env=init \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     atm_merge.atm_merge_method=vortexreplace analysis_merge.analysis_merge_method=vortexreplace \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3db_cycst: atm_init+atm_merge+fgat+3denvar+anal_merge and cycling storm
 confh3db_cycst="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_cycst \
     config.run_atm_init=yes config.run_atm_init_fgat=yes config.run_atm_init_ens=no \
     config.run_atm_merge=yes config.run_atm_merge_fgat=yes config.run_atm_merge_ens=no \
     config.run_atm_vi=no config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=yes config.run_fgat=yes config.run_envar=yes \
     config.gsi_d01=yes config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     atm_merge.atm_merge_method=vortexreplace analysis_merge.analysis_merge_method=vortexreplace \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3db_cycdm: atm_init+atm_merge+fgat+3denvar+anal_merge and cycling whole domain
 confh3db_cycdm="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_cycdm \
     config.run_atm_init=yes config.run_atm_init_fgat=yes config.run_atm_init_ens=no \
     config.run_atm_merge=yes config.run_atm_merge_fgat=yes config.run_atm_merge_ens=no \
     config.run_atm_vi=no config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=yes config.run_fgat=yes config.run_envar=yes \
     config.gsi_d01=yes config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     atm_merge.atm_merge_method=domainmerge analysis_merge.analysis_merge_method=domainmerge \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3db_vicycst: atm_init+atm_vi+anal_merge and cycling storm
 confh3db_vicycst="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_vicycst \
     config.run_atm_init=yes config.run_atm_init_fgat=no config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=yes config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=no config.run_fgat=no config.run_envar=no \
     config.gsi_d01=no config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     vi.vi_storm_env=pert \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     atm_merge.atm_merge_method=vortexreplace analysis_merge.analysis_merge_method=vortexreplace \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3db_vi: atm_init+atm_vi and cycling storm perturbation
 confh3db_vi="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3db_vi \
     config.run_atm_init=yes config.run_atm_init_fgat=no config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=yes config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=no config.run_fgat=no config.run_envar=yes \
     config.gsi_d01=no config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     vi.vi_storm_env=init \
     config.run_analysis_merge=yes config.run_analysis_merge_ens=no \
     atm_merge.atm_merge_method=vortexreplace analysis_merge.analysis_merge_method=vortexreplace \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # h3da_init: warmstart from the coldstart atm_init (initialized from gfs analysis)
 confh3da_init="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3da_init \
     config.run_atm_init=yes config.run_atm_init_fgat=no config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=no config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=no config.run_fgat=no config.run_envar=no \
     config.gsi_d01=no config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     config.run_analysis_merge=no config.run_analysis_merge_ens=no \
     config.NHRS=126 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafs_hycom.conf"

 # h3da: coldstart from gfs analysis directly
 confh3da="config.EXPT=${EXPT} config.SUBEXPT=${EXPT}_h3da \
     config.run_atm_init=no config.run_atm_init_fgat=no config.run_atm_init_ens=no \
     config.run_atm_merge=no config.run_atm_merge_fgat=no config.run_atm_merge_ens=no \
     config.run_atm_vi=no config.run_atm_vi_fgat=no config.run_atm_vi_ens=no \
     config.run_gsi_vr=no config.run_gsi_vr_fgat=no config.run_gsi_vr_ens=no \
     config.run_gsi=no config.run_fgat=no config.run_envar=no \
     config.gsi_d01=no config.gsi_d02=no \
     config.run_ensda=no config.ENS_SIZE=40 config.run_enkf=no \
     config.run_analysis_merge=no config.run_analysis_merge_ens=no \
     config.NHRS=126 \
     forecast.restart_interval=240 \
     config.GRID_RATIO_ENS=2 \
     gsi.use_bufr_nr=yes \
     ../parm/hafsv0p3_regional_storm.conf \
     ../parm/hafsv0p3_hycom.conf"

 # Choose the configuration to run
#confopts="${confh3db_vidacycst}"
#confopts="${confh3db_vida}"
#confopts="${confh3db_cycst}"
#confopts="${confh3db_cycdm}"
#confopts="${confh3db_vicycst}"
#confopts="${confh3db_vi}"
#confopts="${confh3da_init}"
#confopts="${confh3da}"

 confopts="${confh3db_vida}"
 ${PYTHON3} ./run_hafs.py -t ${dev} 2020082506-2020082512 13L HISTORY ${confopts} \
     config.NHRS=12 config.scrub_work=no config.scrub_com=no

 # Storms to run: Laura13L2020, Ida09L2021, Sam18L2021
#${PYTHON3} ./run_hafs.py -t ${dev} 2020081918-2020082718 13L HISTORY ${confopts}
#${PYTHON3} ./run_hafs.py -t ${dev} 2021082612-2021083012 09L HISTORY ${confopts}
#${PYTHON3} ./run_hafs.py -t ${dev} 2021092300-2021100500 18L HISTORY ${confopts}

#===============================================================================

date

echo 'cronjob done'
