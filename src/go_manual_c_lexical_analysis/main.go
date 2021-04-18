package main

import (
	"fmt"
	"go_manual_c_lexical_analysis/parser"
	"io/ioutil"
	"os"
)

func  getRidOfChar13(bytes []byte) []byte{
	var tempByteArray = make([]byte,0)
	for _,ele := range bytes{
		if ele != 13{
			tempByteArray = append(tempByteArray,ele)
		}

	}
	return append(tempByteArray,byte(' '))
}


func main(){
	argNum := len(os.Args)
	if argNum == 1 {
		fmt.Println("请指明需要读入的文件路径")
		return
	}
	var filename = os.Args[1]
	fmt.Print("读入的文件是:" + filename)
	bytes, _ := ioutil.ReadFile(filename)
	fmt.Println("\n************************\n待输入的字符串为:\n"+string(bytes) +"\n************************")
	var myParser = parser.Parser{}
	var temp []byte = getRidOfChar13(bytes)
	myParser.Init(temp )
	for {
		var status = myParser.ParseNext()
		if status == -1{
			myParser.ShowStatus()
			fmt.Println("parse success!")
			break
		}

		if status ==0{
			fmt.Println("parse error!")
			break
		}
		myParser.ShowStatus()
	}

}