#!/bin/tcsh -xef

# ALSO change base volume for align and 3dvolreg!!
set subj = sub03
set ses = pcp_ses03
set base_run = 04
set base_tr = 225
set total_run = 8


set dat_dir = ${subj}/${ses}/func
set base_run = ${dat_dir}/${subj}_r${base_run}

# assign output directory name
set transform_mat_dir = ${subj}/${ses}/transform_mat  # matrix used in motion correction
set reg_dir = ${subj}/${ses}/reg_data  # registered data
set param_dir = ${subj}/${ses}/params  # motion parameters

foreach f_dir ($reg_dir $param_dir $transform_mat_dir)
    if ( ! -d $f_dir ) then
        mkdir $f_dir
        echo making $f_dir
    endif
end

# register and warp
foreach run (`count -digits 2 1 $total_run`)

    # ================================= volreg =================================
    # align each dset to base volume, to anat

    #### register each volume to the base image
    # input: original run files;
    # output 0: run files that are volume registered to the EPI base.
    # output 1: dfile: motion parameters for later regression
    # output 2: 1Dmatrix_save: transformation matrix for the warping
    3dvolreg    -zpad 4 \
                -prefix ${reg_dir}/${subj}_r${run}_vr \
                -dfile ${param_dir}/${subj}_r${run}_vr_mp.1D \
                -base ${base_run}+orig"[$base_tr]" \
                -1Dmatrix_save ${transform_mat_dir}/${subj}_mat_r${run}_vr.aff12.1D   \
                -verbose \
                ${dat_dir}/${subj}_r${run}+orig

end

# #######dfile is the mc param file
# make a single file of registration params
cat ${param_dir}/${subj}_r*_vr_mp.1D > ${param_dir}/${subj}_all_run_vr_mp.1D


