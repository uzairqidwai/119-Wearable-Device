from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QFormLayout, QLineEdit, QTextEdit, \
    QLabel, QDialog, QTreeWidget, QTreeWidgetItem, QHBoxLayout, QMainWindow
import json
import os


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


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Question Config Generator")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)

        self.main_layout = QHBoxLayout(self.central_widget)
        self.layout = QVBoxLayout()

        # Questions tree
        self.questions_tree = QTreeWidget()
        self.questions_tree.setHeaderLabel("Questions and Actions")
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

        self.main_layout.addLayout(self.layout)

        self.root_node = None
        self.root_item = None

        self.update_questions_tree()

    def add_edit_nested_yes(self):
        if not self.root_node:
            self.root_node = QuestionNode()
        dialog = NestedQuestionDialog(self, self.root_node.yes_child)
        if dialog.exec_() == QDialog.Accepted:
            self.root_node.yes_child = dialog.get_data()
            self.update_questions_tree()

    def add_edit_nested_no(self):
        if not self.root_node:
            self.root_node = QuestionNode()
        dialog = NestedQuestionDialog(self, self.root_node.no_child)
        if dialog.exec_() == QDialog.Accepted:
            self.root_node.no_child = dialog.get_data()
            self.update_questions_tree()

    def update_questions_tree(self):
        self.questions_tree.clear()
        if self.root_node:
            self.root_item = QTreeWidgetItem(self.questions_tree, ["Main Question: " + self.root_node.question])
            self.root_item.setData(0, 100, self.root_node)
            self.add_node_to_tree("Yes", self.root_node.yes_child, self.root_item)
            self.add_node_to_tree("No", self.root_node.no_child, self.root_item)
            self.questions_tree.expandAll()

    def add_node_to_tree(self, answer, node, parent_item):
        if node and node.question:
            item = QTreeWidgetItem(parent_item, [answer + ": " + node.question])
            item.setData(0, 100, node)
            self.add_node_to_tree("Yes", node.yes_child, item)
            self.add_node_to_tree("No", node.no_child, item)

    def edit_line(self, item, column):
        node = item.data(0, 100)
        dialog = NestedQuestionDialog(self, node)
        if dialog.exec_() == QDialog.Accepted:
            updated_node = dialog.get_data()
            node.question = updated_node.question
            node.yes_action = updated_node.yes_action
            node.no_action = updated_node.no_action
            self.update_questions_tree()

    def save_config(self):
        config = self.node_to_dict(self.root_node)

        try:
            file_path = os.path.join(os.path.dirname(__file__), 'config.h')
            with open(file_path, 'w') as f:
                f.write(self.generate_config_h(config))
            print("Configuration saved successfully.")
        except Exception as e:
            print("Error saving configuration:", str(e))

        self.update_questions_tree()

    def generate_config_h(self, config):
        return f"""#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R\"({json.dumps(config, indent=4)})\";

#endif // CONFIG_H
"""

    def node_to_dict(self, node):
        if not node or not node.question:
            return None
        return {
            "question": node.question,
            "yes_action": node.yes_action,
            "no_action": node.no_action,
            "yes": self.node_to_dict(node.yes_child),
            "no": self.node_to_dict(node.no_child)
        }


if __name__ == "__main__":
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
