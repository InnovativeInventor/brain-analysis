#!/bin/tcsh -xef

set subj = sub03
set ses = pcp_ses03
set base_r = 04
set base_tr = 225
set total_run = 8

# assign output directory name
set reg_dir = ${subj}/${ses}/reg_data
set detrend_dir = ${subj}/${ses}/detrend_data

set tlrc_dir = ${subj}/${ses}/tlrc_data

if ( ! -d ${tlrc_dir} ) then
  mkdir ${tlrc_dir}
endif

3dAllineate -base ${subj}/anat/${subj}_mprage_warped_resampled+tlrc \
            -source ${reg_dir}/${subj}_r${base_r}_vr+orig"[${base_tr}]" \
            -prefix rm_warped \
            -cmass \
            -overwrite \
            -1Dmatrix_save ${subj}/${ses}/epi2anat


foreach run (`count -digits 2 1 $total_run`)
  
  # 3dresample -master ${subj}/anat/${subj}_mprage_warped_resampled+tlrc\
  #            -prefix ${tlrc_dir}/${subj}_r${run}_final\
  #            -input ${subj}/${ses}/tlrc_data/${subj}_r${run}_final+tlrc

  3dAllineate -base ${subj}/anat/${subj}_mprage_warped_resampled+tlrc \
            -source ${detrend_dir}/${subj}_r${run}_vr_sm_norm_detrend+orig \
            -prefix ${tlrc_dir}/${subj}_r${run}_final \
            -cmass \
            -overwrite \
            -1Dmatrix_apply ${subj}/${ses}/epi2anat.aff12.1D

end




