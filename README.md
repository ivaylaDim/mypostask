<h1 align="center">Gradebook Management System</h1>

<p align="center">
  A C++ command-line application for managing student grades and academic records
</p>

<h2>Project Overview</h2>
<p>Command-line application that simulates the functionality of a gradebook or similar school management system.</p>

<h2>Features</h2>
<ul>
  <li>Student management (CRUD)</li>
  <li>Grade recording and management (CRUD) </li>
  <li>Subject management (CRUD) </li>
  <li>Summaries:
    <ul>
      <li>Average grades for student by subject</li>
      <li>Top performing students</li>
      <li>Students needing makeup exams</li>
      <li>Students with low averages</li>
      <li>Students with birthdays today</li>
    </ul>
  </li>
</ul>

<h2>Requirements</h2>
<ul>
  <li>C++17 or higher</li>
  <li>nlohmann/json library (included)</li>
  <li>Windows operating system (uses <code>direct.h</code> for directory operations)</li>
</ul>


<h2>Usage</h2>
<p>The application provides a menu-driven interface with the following main options:</p>
<ol>
  <li>Student management</li>
  <li>Grades management</li>
  <li>Subject management</li>
  <li>Summaries and reports</li>
</ol>
<p>I have tried my best to add on-screen prompts and displaying guiding information but there may be places that are not very well optimised. I apologise.</p>

<h2>File Structure</h2>
<pre><code>mypostask/
├── main.cpp         # Main application code
├── include/
│   └── json.hpp     # JSON library
├── data/            # Data storage directory
│   ├── Student.json
│   ├── Grade.json
│   ├── Subject.json
│   ├── Teacher.json
│   ├── StudentGrade.json
│   └── Class.json</code></pre>

<h2>JSON Data Storage</h2>
<p>The application stores data in several JSON files:</p>
<ul>
  <li><strong>Student.json</strong>: Student records with ID, name, birthdate, class info</li>
  <li><strong>Grade.json</strong>: Grade definitions with values</li>
  <li><strong>Subject.json</strong>: Subject definitions with associated teachers</li>
  <li><strong>Teacher.json</strong>: Teacher information</li>
  <li><strong>StudentGrade.json</strong>: Linking students to subjects and grades</li>
  <li><strong>Class.json</strong>: Class information</li>
</ul>

<h2>License</h2>
<p>This project is for educational purposes only.</p>
