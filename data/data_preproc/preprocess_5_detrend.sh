#!/bin/tcsh -xef

set subj = sub03
set ses = img_ses03
set total_run = 8

# assign output directory name
set norm_dir = ${subj}/${ses}/norm_data
set detrend_dir = ${subj}/${ses}/detrend_data

if ( ! -d ${detrend_dir} ) then
  mkdir ${detrend_dir}
endif

foreach run (`count -digits 2 1 $total_run`)

  3dDetrend -session ${detrend_dir} \
            -prefix ${subj}_r${run}_vr_sm_norm_detrend \
            -polorts 3 \
            ${norm_dir}/${subj}_r${run}_vr_sm_norm+orig
end