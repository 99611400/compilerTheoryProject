package word

import (
	"fmt"
)

type Operator struct {
	Word
	operatorList []string
}

func (o *Operator) Init(){
	o.wordString = make([]byte,100)
	o.operatorList = []string{"+", "-","*","/","=",">","<",">=","<=","!=","==","!", "%"}
}


func (o *Operator) Parse(str []byte,pointer int) bool{
	var strTemp = str[pointer:]

	if  !o.IsBegin(strTemp[0]){
		return false
	}
	for index,ele := range strTemp{
		if o.IsEnd(ele) {
			o.wordString[index] = 0
			return true
		}
		o.wordString[index] = strTemp[index]
	}
	return false

}

func (o *Operator) format() []byte{
	if o.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return o.Word.wordString
	}
}

func (o *Operator) IsBegin(c byte) bool{
	for _,ele := range  o.operatorList{
		if c == ele[0]{
			return true
		}
	}
	return false
}

func (o *Operator) IsEnd(c byte) bool{
	for _,ele := range  o.operatorList{
		for _,charEle := range ele{
			if c == byte(charEle)  {
				return false
			}
		}
	}
	return true
}

func (o *Operator) WordPrint() {
	fmt.Println("Operator")
}
func (o *Operator) GetTypeCode() int{
	return 4
}

func (o * Operator) GetWordContent() []byte{
	return o.wordString[0:getWordLength(o.wordString)]
}

