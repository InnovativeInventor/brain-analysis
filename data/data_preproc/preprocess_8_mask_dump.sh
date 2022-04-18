#!/bin/tcsh -xef

set subj = sub03
set ses = pcp_ses03
set total_run = 8
set roi_ls = (HIPPO PPA RSC IPS OPA)

# assign output directory name
set tlrc_dir = ${subj}/${ses}/tlrc_data
set norm_dir = ${subj}/${ses}/timeseries_data

if ( ! -d ${norm_dir} ) then
  mkdir ${norm_dir}
endif

foreach roi ($roi_ls)
  foreach run (`count -digits 2 1 $total_run`)

    3dmaskdump -mask masks/${roi}+tlrc \
              -noijk -xyz  \
              -o ${norm_dir}/${subj}_r${run}_${roi}.txt\
              ${tlrc_dir}/${subj}_r${run}_final+tlrc
  end

end

