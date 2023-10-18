# About the project

This is a simple QT Book Store Application.
Application loads books about mobile development from google's API:
https://developers.google.com/books/docs/v1/getting_started#REST
Application fetches and displays thumbnails of a first 20 requested books and loads more as the user scrolls down to the end of the list.
Application have a button to filter/show only books that the user has set as favorite.
When the user clicks on the thumbnail of one of the books, the app shows a detailed information of the book: Title, Author, Description and, if available, a Buy link.
In the detailed view, the user can also favorite / unfavorite a book. This option is stored locally, so it persists through each app usage.
Clicking on the Buy link opens the link on the default system browser.

## Build with

- C++17
- QT 6.5.1
