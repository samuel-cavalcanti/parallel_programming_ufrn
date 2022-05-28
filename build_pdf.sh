#! /bin/zsh


mkdir output
REPO_ROOT=$(pwd)
OUTPUT_DIR=$REPO_ROOT/output

for chapter_dir in chapter_1 chapter_2; do

    for question_dir in $chapter_dir/question_*; do

        cd $(pwd)/$question_dir;
        
        mkdir -p $OUTPUT_DIR/$question_dir


        # echo "$(pwd)"
        pandoc Readme.md -o output.pdf --pdf-engine=xelatex;
        mv output.pdf $OUTPUT_DIR/$question_dir

        cd ../../;

    done

    
done