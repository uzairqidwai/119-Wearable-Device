from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QFormLayout, QLineEdit, QTextEdit, \
    QLabel, QDialog, QTreeWidget, QTreeWidgetItem, QHBoxLayout, QMainWindow, QMessageBox, QTabWidget, QFileDialog
from PyQt5.QtCore import Qt
import json
import os
import sys
import subprocess

class QuestionNode:
    def __init__(self, question="", yes_action="", no_action="", yes_child=None, no_child=None):
        self.question = question
        self.yes_action = yes_action
        self.no_action = no_action
        self.yes_child = yes_child
        self.no_child = no_child


class NestedQuestionDialog(QDialog):
    def __init__(self, parent=None, node=None):
        super().__init__(parent)
        self.setWindowTitle("Add/Edit Nested Question")

        self.node = node if node else QuestionNode()

        self.layout = QVBoxLayout()

        self.form_layout = QFormLayout()

        self.question_input = QTextEdit(self.node.question)
        self.yes_action_input = QLineEdit(self.node.yes_action)
        self.no_action_input = QLineEdit(self.node.no_action)

        self.form_layout.addRow("Question:", self.question_input)
        self.form_layout.addRow("Yes Action:", self.yes_action_input)
        self.form_layout.addRow("No Action:", self.no_action_input)

        self.layout.addLayout(self.form_layout)

        self.add_yes_btn = QPushButton("Add/Edit Nested Question for 'Yes'")
        self.add_yes_btn.clicked.connect(self.add_edit_nested_yes)

        self.add_no_btn = QPushButton("Add/Edit Nested Question for 'No'")
        self.add_no_btn.clicked.connect(self.add_edit_nested_no)

        self.layout.addWidget(self.add_yes_btn)
        self.layout.addWidget(self.add_no_btn)

        self.save_btn = QPushButton("Save")
        self.save_btn.clicked.connect(self.accept)

        self.layout.addWidget(self.save_btn)

        self.delete_btn = QPushButton("Delete")
        self.delete_btn.clicked.connect(self.delete_node)

        self.layout.addWidget(self.delete_btn)

        self.setLayout(self.layout)

    def get_data(self):
        self.node.question = self.question_input.toPlainText()
        self.node.yes_action = self.yes_action_input.text()
        self.node.no_action = self.no_action_input.text()
        return self.node

    def add_edit_nested_yes(self):
        dialog = NestedQuestionDialog(self, self.node.yes_child)
        if dialog.exec_() == QDialog.Accepted:
            self.node.yes_child = dialog.get_data()

    def add_edit_nested_no(self):
        dialog = NestedQuestionDialog(self, self.node.no_child)
        if dialog.exec_() == QDialog.Accepted:
            self.node.no_child = dialog.get_data()

    def delete_node(self):
        self.node = None
        self.accept()


class PhoneNumberWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.layout = QFormLayout()

        self.phone_number_input = QLineEdit()
        self.layout.addRow("Phone Number:", self.phone_number_input)

        self.generate_btn = QPushButton("Generate phonenumber.h")
        self.generate_btn.clicked.connect(self.generate_phonenumber_h)
        self.layout.addWidget(self.generate_btn)

        self.setLayout(self.layout)

    def generate_phonenumber_h(self):
        phone_number = self.phone_number_input.text()
        # Validate phone number
        if not phone_number.isdigit() or len(phone_number) != 10:
            QMessageBox.warning(self, "Invalid Input", "Please enter a valid 10 digit phone number.")
            return
        # Write phone number to phonenumber.h
        try:
            file_path = os.path.join(os.path.dirname(__file__), 'phonenumber.h')
            with open(file_path, 'w') as f:
                f.write('#define PHONE_NUMBER "{}"\n'.format(phone_number))
            QMessageBox.information(self, "Success", "phonenumber.h generated successfully.")
        except Exception as e:
            QMessageBox.critical(self, "Error", "An error occurred while generating phonenumber.h: " + str(e))

class FlashWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.layout = QVBoxLayout()
        
        self.file_input = QLineEdit()
        self.layout.addWidget(self.file_input)
        
        self.browse_btn = QPushButton("Browse")
        self.browse_btn.clicked.connect(self.browse_file)
        self.layout.addWidget(self.browse_btn)
        
        self.flash_btn = QPushButton("Flash")
        self.flash_btn.clicked.connect(self.flash_file)
        self.layout.addWidget(self.flash_btn)
        
        self.setLayout(self.layout)
    
    def browse_file(self):
        fname = QFileDialog.getOpenFileName(self, 'Open file', '/home')
        self.file_input.setText(fname[0])
    
    def flash_file(self):
        file_path = self.file_input.text()
        try:
            subprocess.run(["esptool.py", "--chip", "esp32", "--port", "/dev/ttyUSB0", "--baud", "115200", "--before", "default_reset", "--after", "hard_reset", "write_flash", "-z", "--flash_mode", "dio", "--flash_freq", "40m", "--flash_size", "detect", "0x1000", file_path])
            QMessageBox.information(self, "Success", "File flashed successfully.")
        except Exception as e:
            QMessageBox.critical(self, "Error", "An error occurred while flashing the file: " + str(e))



class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("119 Question Config Generator")
        self.setGeometry(100, 100, 800, 600)

        self.tab_widget = QTabWidget()

        self.central_widget = QWidget(self)
        self.main_layout = QHBoxLayout(self.central_widget)
        self.layout = QVBoxLayout()

        # Questions tree
        self.questions_tree = QTreeWidget()
        self.questions_tree.setHeaderLabels(["Questions", "Actions"])
        self.questions_tree.itemDoubleClicked.connect(self.edit_line)
        self.main_layout.addWidget(self.questions_tree)

        self.form_layout = QFormLayout()

        self.question_input = QTextEdit()
        self.yes_action_input = QLineEdit()
        self.no_action_input = QLineEdit()

        self.form_layout.addRow("Main Question:", self.question_input)
        self.form_layout.addRow("Yes Action:", self.yes_action_input)
        self.form_layout.addRow("No Action:", self.no_action_input)

        self.layout.addLayout(self.form_layout)

        self.add_yes_btn = QPushButton("Add/Edit Nested Question for 'Yes'")
        self.add_yes_btn.clicked.connect(self.add_edit_nested_yes)

        self.add_no_btn = QPushButton("Add/Edit Nested Question for 'No'")
        self.add_no_btn.clicked.connect(self.add_edit_nested_no)

        self.layout.addWidget(self.add_yes_btn)
        self.layout.addWidget(self.add_no_btn)

        self.save_btn = QPushButton("Save Config")
        self.save_btn.clicked.connect(self.save_config)

        self.layout.addWidget(self.save_btn)

        self.delete_btn = QPushButton("Delete Selected Node")
        self.delete_btn.clicked.connect(self.delete_selected_node)

        self.layout.addWidget(self.delete_btn)

        self.main_layout.addLayout(self.layout)

        self.root_node = QuestionNode()  # Initialize the root node
        self.root_item = None

        self.update_questions_tree()

        self.central_widget.setLayout(self.main_layout)

        self.tab_widget.addTab(self.central_widget, "Questions")
        self.tab_widget.addTab(PhoneNumberWidget(), "Phone Number")
        self.setCentralWidget(self.tab_widget)

        self.tab_widget.addTab(FlashWidget(), "Flash")


    def add_edit_nested_yes(self):
        if not self.root_node:
            self.root_node = QuestionNode()
        dialog = NestedQuestionDialog(self, self.root_node.yes_child)
        if dialog.exec_() == QDialog.Accepted:
            self.root_node.yes_child = dialog.get_data()
            self.root_node.question = self.question_input.toPlainText()  # Update main question
            self.update_questions_tree()

    def add_edit_nested_no(self):
        if not self.root_node:
            self.root_node = QuestionNode()
        dialog = NestedQuestionDialog(self, self.root_node.no_child)
        if dialog.exec_() == QDialog.Accepted:
            self.root_node.no_child = dialog.get_data()
            self.root_node.question = self.question_input.toPlainText()  # Update main question
            self.update_questions_tree()

    def update_questions_tree(self):
        self.questions_tree.clear()

        # Explicitly set column count and headers
        self.questions_tree.setColumnCount(2)
        self.questions_tree.setHeaderLabels(["Question", "Action"])

        if self.root_node and self.root_node.question:
            self.root_item = QTreeWidgetItem(self.questions_tree, ["Main Question: " + self.root_node.question])
            self.root_item.setData(0, 100, self.root_node)
            self.add_node_to_tree("Yes", self.root_node.yes_child, self.root_item)
            self.add_node_to_tree("No", self.root_node.no_child, self.root_item)
            self.questions_tree.expandAll()

            # Resize columns for visibility
            self.questions_tree.resizeColumnToContents(0)
            self.questions_tree.resizeColumnToContents(1)

    def add_node_to_tree(self, answer, node, parent_item):
        if node:
            item_text = f"{answer}:" if node.question else answer
            action_text_yes = f"Action: {node.yes_action}" if node.yes_action else "Action: None"
            action_text_no = f"Action: {node.no_action}" if node.no_action else "Action: None"
            action_text = action_text_yes if answer == "Yes" else action_text_no

            item = QTreeWidgetItem(parent_item)
            item.setText(0, item_text + " " + node.question)
            item.setText(1, action_text)  # Explicitly set the text for the second column
            item.setData(0, 100, node)

            self.add_node_to_tree("Yes", node.yes_child, item)
            self.add_node_to_tree("No", node.no_child, item)

    def edit_line(self, item, column):
        node = item.data(0, 100)
        dialog = NestedQuestionDialog(self, node)
        if dialog.exec_() == QDialog.Accepted:
            updated_node = dialog.get_data()
            if updated_node:
                node.question = updated_node.question
                node.yes_action = updated_node.yes_action
                node.no_action = updated_node.no_action
            else:
                parent_item = item.parent()
                if parent_item:
                    parent_node = parent_item.data(0, 100)
                    child_index = parent_item.indexOfChild(item)
                    if child_index == 0:
                        parent_node.yes_child = None
                    else:
                        parent_node.no_child = None
                else:
                    self.root_node = QuestionNode()
            self.update_questions_tree()

    def delete_selected_node(self):
        selected_items = self.questions_tree.selectedItems()
        if selected_items:
            selected_item = selected_items[0]
            parent_item = selected_item.parent()
            if parent_item:
                parent_node = parent_item.data(0, 100)
                child_index = parent_item.indexOfChild(selected_item)
                if child_index == 0:
                    parent_node.yes_child = None
                else:
                    parent_node.no_child = None
                self.update_questions_tree()
            else:
                self.root_node = QuestionNode()
                self.update_questions_tree()

    def save_config(self):
        config = self.node_to_dict(self.root_node)

        try:
            file_path = os.path.join(os.path.dirname(__file__), 'config.h')
            with open(file_path, 'w') as f:
                f.write(self.generate_config_h(config))
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("Configuration saved successfully.")
            msg.setWindowTitle("Success")
            msg.exec_()
        except Exception as e:
            print("Error saving configuration:", str(e))

        self.update_questions_tree()

    def generate_config_h(self, config):
        config_str = json.dumps(config, indent=4)
        return f"""#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R\"({config_str})\";

#endif // CONFIG_H
"""

    def node_to_dict(self, node):
        if node is None:
            return None

        data = {
            "question": node.question if node.question else "",
            "yes_action": node.yes_action if node.yes_action else "",
            "no_action": node.no_action if node.no_action else ""
        }

        if node.yes_child is not None:
            data["yes"] = self.node_to_dict(node.yes_child)

        if node.no_child is not None:
            data["no"] = self.node_to_dict(node.no_child)

        return data


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
