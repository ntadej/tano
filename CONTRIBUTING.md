# How to contribute to Tano

## Feature Requests

If you can, you are encouraged to write the new features by yourself and submit
a pull request (more details below).

Please do not send send feature requests and bug reports via e-mail!


## Submitting Issues

Found a bug in Tano? Here are some guidelines on how to report the issue so
it can be resolved it as fast as possible:

- Explain in detail how to reproduce the issue.
- Please include information on what Tano version you are using and what
  operating system you are running.

Your issue will be closed when not replied for a month or longer. Feel free to
reopen it later if still encountering the problem.


## Pull Requests

To ease the pull request merge into mainline, please follow these requirements:

- The pull request title and message should be meaningful enough that reading
  the code is not necessary.
- **One pull request per feature**. If you want to do more than one thing, send
  multiple pull requests. You should create a separate branch for each one.
- Make sure each individual commit in your pull request is meaningful.
  If you had to make multiple intermediate commits while developing, please
  squash them before sending them to us.
- Use git's [interactive rebase](https://help.github.com/articles/interactive-rebase)
feature to tidy up your commits before submitting the pull request.
- Your PRs must pass build tests on Travis and AppVeyor. We can assist you
  with support for other platforms that you work on, so please note where you
  tested the code before submitting the PR.
