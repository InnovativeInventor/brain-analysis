#!/bin/tcsh -xef

###################################
# Stop Thinking of Forget Items: (ST)
# # 101, exclude for now
###################################

# AFNI message board: The align_epi_anat.py script will take care of obliquity by default, so deobliquing separately is not necessary.

# find out which volume this minimum outlier volume is, and decide whether to use this volume for the run.
# repeat this for all runs of all subs.


set subj = sub03

# assign output directory name
set output_dir = ${subj}/img_ses03/outlier
set data_dir = ${subj}/img_ses03/func

if ( ! -d $output_dir ) then
	mkdir $output_dir
    echo making $output_dir
endif

# ========================== outcount ==========================

foreach run (`count -digits 2 1 8`)
    3dToutcount -automask 	\
    			-fraction 	\
    			-polort 3 	\
    			-legendre  	\
                ${data_dir}/${subj}_r${run}+orig > ${output_dir}/outcount_r${run}.1D

    # censor outlier TRs per run, ignoring the first 0 TRs
    # - censor when more than 0.05 of automask voxels are outliers
    # - step() defines which TRs to remove via censoring
    1deval 	-a ${output_dir}/outcount_r${run}.1D \
    		-expr "1-step(a-0.05)" \
    		> rm_out_censor_r${run}.1D
end

# catenate outlier counts into a single time series
cat ${output_dir}/outcount_r*.1D > ${output_dir}/outcount_allrun.1D

# catenate outlier censor files into a single time series
cat rm_out_censor_r*.1D > ${output_dir}/outcount_${subj}_censor.1D


set tr_counts = ( 227 227 227 227 227 227 227 227)
touch ${output_dir}/out_min_outlier.txt

# get run number and TR index for minimum outlier volume
set minindex = `3dTstat -argmin -prefix - \
						${output_dir}/outcount_allrun.1D\'`

set outlier_vals = ( `1d_tool.py -set_run_lengths $tr_counts -index_to_run_tr $minindex` )
# save run and TR indices for extraction of vr_base_min_outlier
set minoutrun = $outlier_vals[1]
set minouttr  = $outlier_vals[2]
echo "min outlier: run $minoutrun, TR $minouttr" | tee ${output_dir}/out_min_outlier.txt

\rm -f rm_*


