package word

import "fmt"

type UnsignedInteger struct {
	Word
}





func (d *UnsignedInteger) Parse(str []byte,pointer int) bool{
	if !d.IsBegin(str[pointer]){
		return false
	}
	var tempStr = str[pointer:]

	for  index,c := range tempStr{
		if d.IsEnd(c){
			d.wordString[index] = 0
			return true
		}
		if tempStr[index] >= '0' && tempStr[index] <= '9'{
			d.wordString[index] = tempStr[index]
		} else {
			return false
		}
	}
	return true
}

func (d *UnsignedInteger) format() []byte{
	if d.Word.wordString[0] == '\n' {
		return []byte("\\n")
	}else {
		return d.Word.wordString
	}
}

func (d *UnsignedInteger) IsBegin(c byte) bool{

	return c >= '0' && c <= '9'
}

func (d *UnsignedInteger) IsEnd(c byte) bool{
	return !d.IsBegin(c) &&
		!(c >= 'a' && c <= 'z') &&
		!(c >= 'A' && c <= 'Z')
}

func (d *UnsignedInteger) WordPrint() {
	fmt.Println("unsignedInteger print")
}
func (d *UnsignedInteger) GetTypeCode() int{
	return 3
}


func (d *UnsignedInteger) GetWordContent() []byte{
	return d.wordString[0:getWordLength(d.wordString)]
}