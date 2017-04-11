package main

import "os"

func main() {
	if (len(os.Args) < 3){
		println("Error");
	}
	else {
		println(os.Args[0])
	}
}
