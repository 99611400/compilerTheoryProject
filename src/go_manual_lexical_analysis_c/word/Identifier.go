package word

import (
	"fmt"
)


type Identifier struct {
	Word
	reservedWord ReservedWord
}





func (i *Identifier) Parse(str []byte,pointer int) bool{

	var strTemp = str[pointer:]

	if i.reservedWord.Parse(str,pointer){
		return false
	}
	if !i.IsBegin(strTemp[0]){
		return false
	}

	for index,c := range strTemp{
		if  i.IsEnd(c){
			i.wordString[index] = 0
			return true
		}
		i.wordString[index] = strTemp[index]
	}
	return true

}


func (i *Identifier) format() []byte{
	if i.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return i.Word.wordString
	}
}

func (i *Identifier) IsBegin(c byte) bool{
	if (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'){
		return true
	}
	return false
}

func (i *Identifier) IsEnd(c byte) bool{
	return !(
		(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9'))
}

func (i *Identifier) WordPrint() {
	fmt.Println("blankword")
}
func (i *Identifier) GetTypeCode() int{
	return 2
}

func (i *Identifier) GetWordContent() []byte{
	return i.wordString[0:getWordLength(i.wordString)]

}

