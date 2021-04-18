package word

import (
	"fmt"
)


type BlankWord struct {
	Word
}





func (bw *BlankWord) Parse(str []byte,pointer int) bool{
	if str[pointer]== ' '{
		bw.wordString[0] = ' '
		bw.wordString[1] = 0
		return true
	}
	if str[pointer]== 10{
		bw.wordString[0] = 'n'
		bw.wordString[1] = 0
		return true
	}
	return false
}

func (bw *BlankWord) format() []byte{
	if bw.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return bw.Word.wordString
	}
}

func (bw *BlankWord) IsBegin(c byte) bool{
	if c == ' ' || c =='\n' || c== '\t'{
		return true
	}
	return false
}

func (bw *BlankWord) IsEnd(c byte) bool{
	return !bw.IsBegin(c)
}

func (bw *BlankWord) WordPrint() {
	fmt.Println("blankword")
}
func (bw *BlankWord) GetTypeCode() int{
	return 0
}

func (bw * BlankWord) GetWordContent() []byte{
	if bw.wordString[0] == ' ' {
		return  []byte(" ")
	}else {
		return  []byte("\\n")

	}

}

