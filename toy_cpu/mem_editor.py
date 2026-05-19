import sys
from PyQt6.QtWidgets import (QApplication, QMainWindow, QTableWidget, QTableWidgetItem, QTableWidgetItem, QVBoxLayout, QPushButton, 
                             QWidget, QFileDialog)
from PyQt6.QtCore import Qt

MEM_SIZE = 256;

class MemoryEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("CPU Memory Editor")
        self.resize(300, 500)

        main_widget = QWidget()
        layout = QVBoxLayout()

        # Create a table with 256 rows (0x00 to 0xFF) and 2 columns
        self.table = QTableWidget(MEM_SIZE, 2)
        self.table.setHorizontalHeaderLabels(["Mem Location", "Mem Value"])

        for i in range(MEM_SIZE):
            addr_str = f"0x{i:02X}"
            
            # Create the table cells (Items)
            addr_item = QTableWidgetItem(addr_str)
            val_item = QTableWidgetItem("00") # Default memory value

            # Make the Address column read-only so the user can't break it
            addr_item.setFlags(Qt.ItemFlag.ItemIsEnabled) 

            # Put the items into the table at row 'i', column 0 and 1
            self.table.setItem(i, 0, addr_item)
            self.table.setItem(i, 1, val_item)

        # Put the table in the center of the window
        self.setCentralWidget(self.table)

        layout.addWidget(self.table)

        # Create a Save Button
        self.save_button = QPushButton("Save Program")
        self.save_button.clicked.connect(self.save_to_file)
        layout.addWidget(self.save_button)

        # Create Load Buttom
        self.load_button = QPushButton("Load Program");
        self.load_button.clicked.connect(self.load_file)
        layout.addWidget(self.load_button)

        # Apply the layout to the main widget, and set it as the central widget
        main_widget.setLayout(layout)
        self.setCentralWidget(main_widget)

    def load_file(self):
        file_name, _ = QFileDialog.getOpenFileName(
            self, "Load Memory Dump", "", "Text Files (*.txt);;All Files (*)"
        )

        if file_name:
            with open(file_name, 'r') as file:
                lines = file.readlines()
                
                # Loop exactly MEM_SIZE times (0 to 255)
                for i in range(MEM_SIZE):
                    if i < len(lines):
                        value_str = lines[i].strip()
                        
                        self.table.item(i, 1).setText(value_str)
                        
                    else:
                        break
                        
            print(f"Successfully loaded from {file_name}")

    def save_to_file(self):
        # Open a "Save As" dialog so the user can choose where to save the file
        file_name, _ = QFileDialog.getSaveFileName(
            self, 
            "Save Memory Dump", 
            "", 
            "Text Files (*.txt);;All Files (*)"
        )

        # If the user actually chose a file (didn't hit cancel)
        if file_name:
            # Open the file in 'w' (write) mode
            with open(file_name, 'w') as file:
                # Loop through all 256 rows
                for row in range(self.table.rowCount()):
                    # Get the text from the address column (0) and value column (1)
                    address = self.table.item(row, 0).text()
                    value = self.table.item(row, 1).text()
                    
                    # Write them to the file separated by a space, followed by a newline
                    file.write(f"{value}\n")
            
            print(f"Successfully saved to {file_name}")

if __name__ == "__main__":
    app = QApplication(sys.argv)  
    window = MemoryEditor()       
    window.show()                 
    sys.exit(app.exec())          
