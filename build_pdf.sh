#! /bin/zsh


mkdir output
REPO_ROOT=$(pwd)
OUTPUT_DIR=$REPO_ROOT/output

for chapter_dir in chapter_1 chapter_2; do

    for question_dir in $chapter_dir/question_*; do

        cd $(pwd)/$question_dir;
        
        # mkdir -p $OUTPUT_DIR/$question_dir


        # echo "$(pwd)"
        # pandoc Readme.md -o output.pdf --pdf-engine=xelatex;
        # mv output.pdf $OUTPUT_DIR/$question_dir

        cd ../../;

    done

    
done

## código para executar com uma linha
## lembre-se de colocar as fotos na raiz do repo
##pandoc chapter_1/question_1/Readme.md  chapter_1/question_2/Readme.md chapter_1/question_3/Readme.md chapter_1/question_4/Readme.md chapter_1/question_5/Readme.md chapter_1/question_6/Readme.md chapter_1/question_9/Readme.md chapter_2/question_1/Readme.md   chapter_2/question_2/Readme.md chapter_2/question_3/Readme.md chapter_2/question_5/Readme.md chapter_2/question_7/Readme.md chapter_2/question_10/Readme.md chapter_2/question_15/Readme.md chapter_2/question_16/Readme.md chapter_2/question_17/Readme.md chapter_2/question_19/Readme.md chapter_2/question_20/Readme.md chapter_2/question_21/Readme.md chapter_2/question_24/Readme.md -o  output.pdf --pdf-engine=xelatex