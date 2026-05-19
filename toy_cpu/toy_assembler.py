import sys

def read_file(filename):
    f = open(filename, "r")
    content = f.read()
    f.close()
    return content

def lexer(program):
    tokens = program.split()
    return tokens

def hex_file(tokens):
    hex_list = []
    token_stream = iter(tokens)
    
    def get_next_arg(instruction_name, context=""):
        try:
            arg = next(token_stream)
            return int(arg, base=0)
        except StopIteration:
            ctx_str = f" ({context})" if context else ""
            print(f"Error: {instruction_name} instruction missing an argument at end of file!{ctx_str}")
            return None 

    for token in token_stream:
        if token == 'LDA':
            hex_list.append(0x01)
            hex_list.append(get_next_arg('LDA'))
        
        elif token == 'LDI':
            hex_list.append(0x02)
            hex_list.append(get_next_arg('LDI'))
            
        elif token == 'ADDA':
            hex_list.append(0x03)
            hex_list.append(get_next_arg('ADDA'))
        
        elif token == 'ADDI':
            hex_list.append(0x04)
            hex_list.append(get_next_arg('ADDI'))
            
        elif token == 'STA':
            hex_list.append(0x05)
            hex_list.append(get_next_arg('STA'))
            
        elif token == 'END':
            hex_list.append(0x06)
            
        elif token == 'GOTO':
            hex_list.append(0x07)
            
        elif token == 'BREQ':
            hex_list.append(0x08)
            hex_list.append(get_next_arg('BREQ', 'comparison'))
            hex_list.append(get_next_arg('BREQ', 'destination'))
            
        else:
            hex_list.append(token)
            
    return hex_list

def write_to_file(hex_list):
    filename = "program.txt"
    
    if len(sys.argv) >= 4 and sys.argv[2] == "-o":
        filename = sys.argv[3]
    elif len(sys.argv) == 3 and sys.argv[2] == "-o":
        print("Error: -o flag requires an output filename!")
        sys.exit(1)

    with open(filename, "w") as file:
        for addr in hex_list:
            if isinstance(addr, int):
                hex_string = f"0x{addr:02X}"
                file.write(hex_string + "\n") 

def main():
    if len(sys.argv) < 2:
        print("Please enter a file!")
        sys.exit(1)

    program = read_file(sys.argv[1])
    tokens = lexer(program)
    hex_list = hex_file(tokens)
    write_to_file(hex_list)

if __name__ == "__main__":
    main()

