
def cases(argument):
    switcher = {
        0: "R000000",
        1: "L100100",
        2: "L000100",
    }
    return switcher.get(argument, "nothing")

def sending(loop):
	val = 0
	moveCase = 0
	while True:
		val += 1
		print(cases(moveCase))
		if val>loop:
			val = 0
			moveCase += 1
		if moveCase>2:
			break

if __name__=='__main__':
	sending(1000)
