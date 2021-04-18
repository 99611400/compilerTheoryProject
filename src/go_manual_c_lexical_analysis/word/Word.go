package word

import (
	"fmt"
)

type InterfaceOfWordSon interface {
	Parse(string []byte,pointer int) bool
	IsBegin(c byte) bool
	IsEnd(c byte) bool
	WordPrint()
	GetTypeCode() int
	StepIncrease() int
}


type  Word struct {
	wordString []byte
}



func (w *Word) Init(){
	w.wordString = make([]byte,100)
}

func getWordLength(bytes []byte)  int{
	var end int = 0
	_ = end
	for index,c := range bytes{
		if c == 0{
			end = index
			break
		}
	}
	return end
}


func (w *Word) StepIncrease() int{
	return getWordLength(w.wordString)
}

func (w *Word) Parse(string []byte,pointer int) bool {
	fmt.Println("word的Parser函数执行")
	return false
}

func (w *Word) IsBegin(c byte) bool{
	fmt.Println("word的IsBegin函数执行")
	return false
}

func (w *Word) IsEnd(c byte) bool{
	fmt.Println("word的IsEnd函数执行")
	return false
}



func (w *Word) GetTypeCode() int {
	fmt.Println("word的GetTypeCode函数执行")
	return -1
}
