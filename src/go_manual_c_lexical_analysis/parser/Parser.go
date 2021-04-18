package parser

import (
	"fmt"
	"go_manual_c_lexical_analysis/word"
	"strconv"
)




type Parser struct {
	pointer      int
	parsedString []byte
	words        []interface{}
	nowWord      []byte
	nowType      int
}


func (p *Parser) Init(str []byte) {
	p.parsedString = make([]byte,0)
	p.parsedString = str
	myBlankWord := word.BlankWord{}
	myBlankWord.Init()
	p.words = append(p.words, myBlankWord)

	myUnsignedInteger := word.UnsignedInteger{}
	myUnsignedInteger.Init()
	p.words = append(p.words, myUnsignedInteger)


	myOperator := word.Operator{}
	myOperator.Init()
	p.words = append(p.words, myOperator)


	myReservedWord := word.ReservedWord{}
	myReservedWord.Init()
	p.words = append(p.words,myReservedWord)

	mySeparator := word.Separator{}
	mySeparator.Init()
	p.words = append(p.words,mySeparator)

	myIdentifier := word.Identifier{}
	myIdentifier.Init()
	p.words = append(p.words,myIdentifier)

	p.nowWord = nil
}








func (p *Parser) ParseNext() int{
	var parseSuccess bool = false
	for _,tempWord := range p.words{

		if parseSuccess == true{
			break
		}
		switch tempWord.(type) {
			case word.BlankWord:
				tempWord ,_ := tempWord.(word.BlankWord)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			case word.UnsignedInteger:
				tempWord,_ := tempWord.(word.UnsignedInteger)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			case word.Operator:
				tempWord,_ := tempWord.(word.Operator)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			case word.ReservedWord:
				tempWord,_ := tempWord.(word.ReservedWord)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			case word.Separator:
			tempWord,_ := tempWord.(word.Separator)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			case word.Identifier:
				tempWord,_ := tempWord.(word.Identifier)
				if tempWord.Parse(p.parsedString ,p.pointer){
					parseSuccess = true
					p.pointer += tempWord.StepIncrease()
					p.nowWord = tempWord.GetWordContent()
					p.nowType = tempWord.GetTypeCode()
				}else{
					parseSuccess = false
				}
			default:
				fmt.Println("默认")

		}

	}



	if parseSuccess && p.pointer >= len(p.parsedString){
		return -1
	}
	if parseSuccess {
		return 1
	}
	return 0
}



func (p *Parser) ShowStatus(){
	if p.nowType != 0{
		fmt.Println("{" + "\""  + string(p.nowWord)  +"\"" +","  + "\""  + strconv.Itoa( p.nowType)   +"\""    + "}$")
	}
}