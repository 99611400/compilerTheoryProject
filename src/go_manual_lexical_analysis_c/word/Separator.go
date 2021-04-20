package word

import (
	"fmt"
)

type Separator struct {
	Word
	separatorList []string
}

func (s *Separator) Init(){
	s.wordString = make([]byte,100)
	s.separatorList = []string{",",";","{","}","(",")"}
}


func (s *Separator) Parse(str []byte,pointer int) bool{
	var strTemp = str[pointer:]
	if  s.IsBegin(strTemp[0]){
		s.wordString[0] = strTemp[0]
		s.wordString[1] = 0
		return true
	}

	return false

}

func (s *Separator) format() []byte{
	if s.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return s.Word.wordString
	}
}

func (s *Separator) IsBegin(c byte) bool{
	for _,ele := range  s.separatorList{
		if c == ele[0]{
			return true
		}
	}
	return false
}

func (s *Separator) IsEnd(c byte) bool{
	return !s.IsEnd(c)
}

func (s *Separator) WordPrint() {
	fmt.Println("Operator")
}
func (s *Separator) GetTypeCode() int{
	return 5
}

func (s *Separator) GetWordContent() []byte{
	return s.wordString[0:getWordLength(s.wordString)]
}

