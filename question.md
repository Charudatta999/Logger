📚 Chapter 4: Memory & Smart Pointers
Task: Implementing a Logger with RAII
Goal:
We’re going to make RAII work for us by creating a Logger class! The concept is pretty simple:

When a Logger object is created, it opens a log file. When it’s destroyed (like when your program ends or the object goes out of scope), the file automatically closes.

Think of it like a waiter who opens the door for you when you enter a restaurant and closes it when you leave — no extra effort, just automatic!

Real-World Scenario:
Every app or program needs to log information at some point. Whether you're recording important events, tracking errors, or debugging, logs help us keep track of everything happening in the application.

But what if your app could automatically open a log file whenever it needs to write to it — and just automatically close the file when it's done? Sounds cool, right? That’s the magic of RAII!

RAII ties resources to an object’s lifetime, so when the object goes out of scope, the resource (like a log file) is automatically cleaned up. In this task, we’ll be building a logger that opens and closes a log file, using RAII principles. 🌟

Task Breakdown:
Step 1: Implement Logger with Raw File Management
💻 What you’ll do:
Create a simple Logger class that manually opens a file in the constructor and closes it in the destructor. This is the old-school way — you’ll experience firsthand how easy it is to forget to close the file!

🔍 Goal:
You’ll see how manual resource management can be risky. Forgetting to close a file? Oh no! 😱

Step 2: Refactor Logger Using std::unique_ptr
💡 What you’ll do:
Refactor your Logger class to use a std::unique_ptr for managing the file handle. With RAII, when the logger object goes out of scope, the file will automatically close. No manual cleanup needed!

🔑 Goal:
Learn how std::unique_ptr ties the file to the lifetime of the object, ensuring automatic cleanup.

Step 3: Implement RAII for File Resource Management
🎯 What you’ll do:
Fully embrace RAII by creating a Logger class that opens the file on creation and closes it when the object is destroyed. The best part? No more worrying about manual cleanup! 😎

🚀 Goal:
Experience how RAII works its magic — the file opens when the logger is created and automatically closes when the logger goes out of scope. Magic!

Step 4: Use std::shared_ptr for Shared Logging Access
👥 What you’ll do:
Now that you’ve got std::unique_ptr down, let’s take it to the next level! Use std::shared_ptr to allow multiple parts of your application to share access to the same log file. 📂

🧐 Goal:
Learn how std::shared_ptr lets multiple parts of your program share ownership of the log file, and still ensures automatic cleanup when no one is using it.

Expected Outcomes:
1. Manual Resource Management with Raw File Handles
Before jumping into smart pointers, you'll start by manually handling the log file. You'll experience the risks of forgetting to close a file (oops!). You’ll get why RAII is so cool for automatic resource management. ❌📂

2. Using std::unique_ptr for RAII
Next, you’ll refactor the code to use std::unique_ptr for resource management. With RAII, the file will automatically be closed when the logger goes out of scope. 🎩✨

3. RAII for Automatic Cleanup
You’ll see how RAII ties the log file to the lifetime of the Logger object. As soon as the Logger is destroyed, the file gets closed automatically. It’s like having your own personal assistant for resource management! 🤖

4. Using std::shared_ptr for Shared Access
What if you need multiple parts of your program to share the same log file? You’ll use std::shared_ptr for shared ownership. With shared pointers, the file stays open as long as it’s being used and is automatically closed when no one is accessing it. 👫📂

Real-World Benefits:
1. Automatic Resource Management
By using RAII with smart pointers, you can ensure your log file is opened when the logger is created and closed when the logger is destroyed. No more worrying about forgetting to close the file! 🎉

2. Prevention of Resource Leaks
Without RAII, it’s easy to forget to close a file, which leads to resource leaks. With smart pointers, your file will always be cleaned up properly, no matter what happens. ✅

3. Error-Free Logging
With RAII, your logging system becomes bulletproof. Forgetting to open or close the log file? Not anymore! Your logs are consistent and error-free. 🛡️

4. Easy-to-Maintain Code
Once you get the hang of using smart pointers, your code becomes cleaner, safer, and easier to maintain. You don’t need to worry about manually managing memory or file handles. It’s all automated! 🔧💡

Purpose of the Task:
1. Understand RAII
You’ll see how RAII works, where resources (like files) are tied to the lifetime of objects. As soon as the object is destroyed, the resource is cleaned up. No extra effort needed. ✨

2. Familiarize with Smart Pointers
By using std::unique_ptr and std::shared_ptr, you’ll become comfortable with smart pointers, which are crucial for memory management and resource safety in modern C++. 🧠

3. Learn How RAII Can Solve Real-World Problems
RAII is not just a cool concept — it’s a practical solution for managing resources in real-world applications. By building a logging system, you’ll see how RAII works in action! 🚀

Deliverables:
A Logger Class with RAII
Implement a Logger class that opens a log file when created and automatically closes it when the object is destroyed.

Use of Smart Pointers
Refactor the code to use std::unique_ptr and std::shared_ptr to manage the file handle and ensure automatic cleanup.

Real-World Logging Simulation
Build a logging system where multiple components of your application can log to the same file, showing how RAII ensures the file is automatically managed