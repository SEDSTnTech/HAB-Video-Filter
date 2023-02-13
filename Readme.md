
# README

## High-Altitude Ballooning (HAB) Video Filter

Simple program that will cut out frames that are above a certain blue threshold

The purpose of this program is to cut out portions of HAB footage that contain mostly sky as these clips are visually uninteresting.
This should help speed up the process of editing together HAB footage into useable videos for outreach or other purposes.

Must run script with arguments in command line as follows without brackets:

    python3 VideoFilter.py [input_filename.mp4] [output_filename.mp4] [blue_threshold (float)]

* input_filename      -   name of the original file to be cut down, can be a file in local directory or path to file
* output_filename     -   name of the file that the program will name the cut file, can also be a path to file
* blue_threshold      -   fraction of frame that must be blue in order to be cut (ex: more than 40% -> 0.4, more than 90% -> 0.9)

Example:

    python3 VideoFilter.py input.mp4 output.mp4 0.3

As of right now the .mp4 does need to be included on the end of each file name

This example cuts any frames from input.mp4 that are above 30% blue and outputs it to a file names output.mp4

The program will also record and print out the timestamps of the clips used form the original video

Example:
00:00:05 - 00:00:30
00:00:45 - 00:01:30

This would indicate that a clip was taken between 5s and 30s and another between 45s and 1min 30s and both of these clips
were added to the new video file
