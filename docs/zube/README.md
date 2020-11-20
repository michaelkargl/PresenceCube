# Website

This website is built using [Docusaurus 2](https://v2.docusaurus.io/), a modern static website generator.

## Reading the docs

There are 3 ways of consuming these docs

1. Run the full docusaurus instance and browse them using the web-interface
1. Export the docs to PDF
1. [Browse the Markdown files manually](./docs)

## Installation

After checkin out the repository, you'll need to install all the dependencies:
```console
npm install
```

## Local Development

```console
npm start
```

This command starts a local development server and open up a browser window. 
Most changes are reflected live without having to restart the server.

## Build

```console
npm build
```

This command generates static content into the `build` directory and can be served using any static contents hosting service.

## Deployment

```console
GIT_USER=<Your GitHub username> USE_SSH=true yarn deploy
```

If you are using GitHub pages for hosting, this command is a convenient way to build the website and push to the `gh-pages` branch.

## PDF Export

You can export the documentation as PDF by running the instance
```console
npm start
```
and issue
```console
npm run build-pdf
```
within a separate window.
