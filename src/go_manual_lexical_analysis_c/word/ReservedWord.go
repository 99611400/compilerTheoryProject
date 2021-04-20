package word

import (
	"fmt"
)

type ReservedWord struct {
	Word
	reservedWords []string
}

func (r *ReservedWord) Init(){
	r.wordString = make([]byte,100)
	r.reservedWords = []string{ "int","if" ,"for","while","double","do","return","break","continue","else"}
}


func (r *ReservedWord) Parse(str []byte,pointer int) bool{
	var strTemp = str[pointer:]

	if  !r.IsBegin(strTemp[0]){
		return false
	}
	for _,ele := range r.reservedWords{
		if  len(ele) < len(strTemp) && string( strTemp[0:len(ele)]) == ele {
			efg := make([]byte,100)
			copy(efg,strTemp[0:len(ele)] )
			r.wordString = append(efg,0)
			return true
		}
	}
	return false

}

func (r *ReservedWord) format() []byte{
	if r.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return r.Word.wordString
	}
}

func (r *ReservedWord) IsBegin(c byte) bool{
	for _,ele := range  r.reservedWords{
		if c == ele[0]{
			return true
		}
	}
	return false
}

func (r *ReservedWord) IsEnd(c byte) bool{
	//for _,ele := range  r.reservedWords{
	//	for _,charEle := range ele{
	//		if c == byte(charEle)  {
	//			return false
	//		}
	//	}
	//}
	//return true

	return c >= 'a' && c <= 'z' ||
		c >= 'A' && c <= 'Z'

}

func (r *ReservedWord) WordPrint() {
	fmt.Println("Operator")
}
func (r *ReservedWord) GetTypeCode() int{
	return 1
}

func (r *ReservedWord) GetWordContent() []byte{
	return r.wordString[0:getWordLength(r.wordString)]
}

