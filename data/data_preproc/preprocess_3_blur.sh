#!/bin/tcsh -xef

set subj = sub03
set ses = pcp_ses03
set total_run = 8
    
set reg_dir = ${subj}/${ses}/reg_data
set blur_dir = ${subj}/${ses}/blur_data

if ( ! -d $blur_dir ) then
    mkdir $blur_dir
endif

### ================================== blur ==================================
# blur each volume of each run
# you should smooth with a Gaussian filter approximately three times the size of your voxel. 
# If your voxel size is 3 x 3 x 3 mm, you would smooth with a 9 mm FWHM Gaussian filter.

foreach run (`count -digits 2 1 $total_run`)

    3dmerge -1blur_fwhm 2.0  \
            -doall \
            -prefix ${blur_dir}/${subj}_r${run}_vr_sm \
            ${reg_dir}/${subj}_r${run}_vr+orig
end







