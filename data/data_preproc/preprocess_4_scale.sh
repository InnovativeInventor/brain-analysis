#!/bin/tcsh -xef

set subj = sub03
set ses = img_ses03
set total_run = 8
    
# assign output directory name
set blur_dir = ${subj}/${ses}/blur_data
set norm_dir = ${subj}/${ses}/norm_data

if ( ! -d $norm_dir ) then
    mkdir $norm_dir
endif


# ================================= scale ==================================
# scale each voxel time series to have a mean of 100
# (be sure no negatives creep in)
# (subject to a range of [0,200])

###### use our lab's %signal change.

foreach run (`count -digits 2 1 $total_run`)
    
    3dTstat -prefix rm_scale_r${run}_mean \
            ${blur_dir}/${subj}_r${run}_vr_sm+orig

    3dcalc  -datum float \
            -a ${blur_dir}/${subj}_r${run}_vr_sm+orig \
            -b rm_scale_r${run}_mean+orig \
            -expr "((a-b)/b*100)" \
            -overwrite \
            -prefix ${norm_dir}/${subj}_r${run}_vr_sm_norm
end

rm rm*







