# fMRI data preprocessing pipeline
This file is to fully explain how we turned the raw fMRI data into the 2 edge lists with correlation coefficients as the weights.

## Data acquisition
The raw data can be downloaded from https://openneuro.org/datasets/ds001506/versions/1.3.1. For the purpose of this project, we only used the ses-anatomy, ses-imagery0* and ses-PerceptionNaturalImageTest0* for all 3 subjects. 

The ses-anatomy has a high-resolution anatomical scan of the brain which serves as the base of alignment later to register all subject's brain into the same space.

The ses-imagery0* has all functional scans of subjects when they are imagining the natural images, and the associated behavioral file.

The ses-PerceptionNaturalImageTest0* has the functional scans of subjects when they are viewing the same set of images.

## Software for fMRI data processing
AFNI is the main tool for preprocessing fMRI data: https://afni.nimh.nih.gov/pub/dist/doc/htmldoc/background_install/main_toc.html. Note that fMRI data cannot be used to extract any valuable information without a series of denoising steps. The parameters and choices are highly dependent on the data itself. In addition, after each step of processing, it is always important to visualize the brain in their GUI to make sure that nothing went awry in the previous step. Therefore, even though the multiple files seem redundant, they are necessary to encourage frequently checking the data.

AFNI commands can be run directly in the terminal. The scripts are for record keeping and making things easier to run.

Storage-wise: the data produced at each step take up a tremendous amount of space: about 338.GB in total after I finished.


## Preprocessing steps

### 0. Converting to BRIK/HEAD
We used 3dcopy commands to turn all nifti (.ii) files into BRIK/HEAD files to be able to work with AFNI. Do this separately for all nii files and visually check that there is in corruption on any one of them. (This is tedious, but necessary when we are using other people's data)

### 1. preprocess_1_decide_base.sh
Run this for each session (e.g. ses-imagery01) to find the base run and base TR with the smallest motion, so that we can use it as the reference volume.

Recorded these volumes for each session of each participants in the sub_info.xlsx


### 2. preprocess_2_volreg.sh
Run this to register all runs of data to the reference volume we had above.

### 3. preprocess_3_blur.sh
Run this to perform spatial smoothing to increase signal-to-noise ratio. We chose a gaussian filter with 2mm kernel. This choice was dependent on the the scanning parameter of our data. They used a 2 by 2 by 2 mm resolution, and since we are interested in each voxel, we just doubled the resolution.

### 4. preprocess_4_scale.sh
Perform a time-series demeaning to turn the data's arbitrary units into %signal change. The raw data from scaner usually have an amplitude of about 1000, but this is a meaningless unit depicting the strength of signal, and they have to be scaled to represent the percent signal change.

### 5. preprocess_5_detrend.sh
One common noise in fMRI is called scanner drift, combined with subjects' motion, there is usually a linear drift in the time series and the solution is to run a regression to get rid of these accountable noise. We chose an order of polort of 3, meaning that we are using the constant, X and X^2 in the regression model to account for the noise. This number is based on the length of a run. Usually we start from 1 and add 1 for every extra 150 seconds. Our run is about 250 seconds, so we added 2 to be safe.

### 6. preprocess_6_warpanat2mni.sh
This is performed for the ses-anatomy in each subject to warp their brain into the a template space: MNI152. We used a nonliearwarp program:SSwarper, this take about 2 hours to run for each warp on my laptop.

### 7. preprocess_7_warpepi2anat.sh
We then warp each functional run to the corresponding anatomy (the ones just went through the warping).

### 7.5 ROI
We are interested in 5 regions of interest: PPA, OPA, cIPL, RSC and hippocampus. They are extracted from the MNI_glasser atlas in AFNI GUI. 

We use 3dresample command in terminal to keep everything on the same coordinates and resolution scale.

### 8. preprocess_8_mask_dump.sh
With the ROI masks and functional runs ready, we can extract the time series of each voxel into text files.

### 9. anal.py
Finally, we extracted the behavioral information from the tsv files and kept only meaningful time points in the time series we just extracted. (For imagery, we only kept the trials with evaluation scores above 2, because 2 means that subjects were not able to imagine/recall the image) in python, and used numpy to calculate the pair wise correlations, kept only the top 3% correlations (excluding teh self correlations) and wrote the edgelist into text files for the 2 conditions.

We also kept a seprate coords.txt file to keep track of these voxels's coordinates so that we can reconstruct them after the graph analysis.




