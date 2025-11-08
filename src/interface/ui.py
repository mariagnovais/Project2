from typing import Optional

import tkinter as tk
from tkinter import ttk


def make_label(parent: tk.Widget, text: str, **grid_kwargs: int) -> tk.Label:
    label = tk.Label(parent, text=text, anchor="w", font=("Courier New", 16), fg="#1d2c79", bg="white")
    label.grid(sticky="ew", pady=(8, 2), **grid_kwargs)
    return label


class FormFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_back) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")
        self.on_back = on_back

        title = tk.Label(self, text="Enter your preferences:", font=("Courier New", 28, "bold"), fg="#1d2c79", bg="white")
        title.grid(row=0, column=0, columnspan=2, sticky="w", pady=(0, 20))

        self.name_var = tk.StringVar()
        self.sat_var = tk.StringVar()
        self.state_var = tk.StringVar()
        self.control_var = tk.StringVar(value="Select Control")
        self.tuition_var = tk.StringVar()
        self.acceptance_var = tk.StringVar()

        make_label(self, "Name:", row=1, column=0)
        name_entry = ttk.Entry(self, textvariable=self.name_var, font=("Courier New", 16))
        name_entry.grid(row=2, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        make_label(self, "SAT Score:", row=3, column=0)
        sat_entry = ttk.Entry(self, textvariable=self.sat_var, font=("Courier New", 16))
        sat_entry.grid(row=4, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        make_label(self, "State:", row=5, column=0)
        state_entry = ttk.Entry(self, textvariable=self.state_var, font=("Courier New", 16))
        state_entry.grid(row=6, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        make_label(self, "Control:", row=7, column=0)
        control_combo = ttk.Combobox(
            self,
            textvariable=self.control_var,
            values=("Select Control", "Public", "Private", "Any"),
            font=("Courier New", 16),
            state="readonly",
        )
        control_combo.grid(row=8, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        make_label(self, "Tuition:", row=9, column=0)
        tuition_entry = ttk.Entry(self, textvariable=self.tuition_var, font=("Courier New", 16))
        tuition_entry.grid(row=10, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        make_label(self, "Acceptance Rate:", row=11, column=0)
        acceptance_entry = ttk.Entry(self, textvariable=self.acceptance_var, font=("Courier New", 16))
        acceptance_entry.grid(row=12, column=0, columnspan=2, sticky="ew", pady=(0, 16))

        button_frame = ttk.Frame(self, style="Card.TFrame")
        button_frame.grid(row=13, column=0, columnspan=2, pady=(16, 0), sticky="e")

        ttk.Button(button_frame, text="Back", command=self.on_back, style="Accent.TButton").grid(row=0, column=0, padx=6)
        ttk.Button(button_frame, text="Submit", command=self.print_summary, style="Accent.TButton").grid(row=0, column=1, padx=6)

        for column in range(2):
            self.columnconfigure(column, weight=1)

        name_entry.focus_set()

    def print_summary(self) -> None:
        summary = (
            f"Name: {self.name_var.get()}\n"
            f"SAT Score: {self.sat_var.get()}\n"
            f"State: {self.state_var.get()}\n"
            f"Control: {self.control_var.get()}\n"
            f"Tuition: {self.tuition_var.get()}\n"
            f"Acceptance Rate: {self.acceptance_var.get()}"
        )
        print("Preferences captured:\n" + summary)


class WelcomeFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_start, on_exit) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")

        title = tk.Label(self, text="College Matcher", font=("Courier New", 36, "bold"), fg="#1936a3", bg="white")
        title.grid(row=0, column=0, pady=(0, 20))

        emoji = tk.Label(self, text="🎓", font=("Arial", 48), bg="white")
        emoji.grid(row=0, column=1, padx=(16, 0), sticky="w")

        start_button = tk.Button(
            self,
            text="Start Matching",
            command=on_start,
            bg="#1936a3",
            fg="white",
            font=("Courier New", 18, "bold"),
            activebackground="#0f1f5c",
            relief="flat",
            padx=40,
            pady=16,
        )
        start_button.grid(row=1, column=0, columnspan=2, pady=(40, 16))

        exit_button = tk.Button(
            self,
            text="Exit",
            command=on_exit,
            bg="#1936a3",
            fg="white",
            font=("Courier New", 18, "bold"),
            activebackground="#0f1f5c",
            relief="flat",
            padx=40,
            pady=16,
        )
        exit_button.grid(row=2, column=0, columnspan=2, pady=(0, 10))

        for column in range(2):
            self.columnconfigure(column, weight=1)

        start_button.focus_set()


class CollegeMatcherApp(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.title("College Matcher")
        self.configure(bg="#ecf0ff")
        self.geometry("960x720")

        style = ttk.Style(self)
        style.configure("Card.TFrame", background="white")
        style.configure("Accent.TButton", font=("Courier New", 16, "bold"), padding=10)

        self.card = ttk.Frame(self, style="Card.TFrame", padding=24)
        self.card.place(relx=0.5, rely=0.5, anchor="center")

        self.current_frame: Optional[ttk.Frame] = None
        self.show_welcome()

    def show_welcome(self) -> None:
        self.swap_frame(WelcomeFrame(self.card, self.show_form, self.destroy))

    def show_form(self) -> None:
        self.swap_frame(FormFrame(self.card, self.show_welcome))

    def swap_frame(self, frame: ttk.Frame) -> None:
        if self.current_frame is not None:
            self.current_frame.destroy()
        self.current_frame = frame
        self.current_frame.grid(row=0, column=0, sticky="nsew")

        self.card.columnconfigure(0, weight=1)
        self.card.rowconfigure(0, weight=1)


def main() -> None:
    app = CollegeMatcherApp()
    app.mainloop()


if __name__ == "__main__":
    main()