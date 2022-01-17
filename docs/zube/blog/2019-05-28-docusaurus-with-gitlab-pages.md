---
slug: today-i-learned-docusaurus-with-gitlab-pages
title: "Today I learned: Docusaurus with Gitlab Pages"
author: Michael Kargl
author_title: Docusaurus Core Team
author_url: https://gitlab.com/michaelkargl
author_image_url: https://secure.gravatar.com/avatar/88023fd552be8a8e8430136472c2e6b0?s=800&d=identicon
tags: [ today-i-learned, docusaurus, gitlab, ci/cd, gitlab-pages, lunrjs, gatsby, algolia, docsearch, static-site-generators, devops ]
---

At times, it is hard to keep track of your own learnings
, findings, notes and achievements. When building a project from scratch, one has to deal with problems as they arise. Sometimes these problems are just small, googleable, problems easily solved in 5 minutes. There are, however, problems that can take weeks, months or even years to solve. These problems often tend to shadow the little things. Today's task was to find a way to document learnings
 big and small to make the invisible more visibl
e....

<!--truncate-->

## Static Site Generators

![](https://docs.gitlab.com/ee/user/project/pages/img/ssgs_pages.png)

A while back I was experiementing a lot with static site generators:

* **Gatsby**
   * Built a little [photography portfolio][osaportfolio_page] with it and felt quite nice and there's a lot of public themes out there. The fact that it is built with react helps with custom development. As my goal was to get a blog up and running within a few hours, I broke off my search for a publicly available blogging template and continued my search.
     * https://gitlab.com/ohsnaparts/osasoftworks/portfolio/osaportfolio
* **Docusaurus**
   * A documentation tool by meta that, I think originally was intended to be a code documentation tool only, but since version 2 it also allows for the creation of blogs. Light and dark mode is always a plus (especially at night :P) but most importantly, it provides great documentation for the most common tasks. What made it stand out for me was that it is very simple to integrate local search through [3rd party open source plugins](#local-search-through-lunrjs).

## Docusaurus

![](https://d33wubrfki0l68.cloudfront.net/ea8e37a6a30e9c260a8936d95c579af4a2dd3df7/6ee7e/img/docusaurus_keytar.svg)

### Blog subfolders

Out of the box, docusaurus file structure is flat. It is not possible to categorize blog posts in subfolders or categories.\
It is however possible to develop this feature yourself
* https://jvdevlab.com/blog/docusaurus/blog-subfolders

### Blog sidebar nesting

Similar to [Blog subfolders](#blog-subfolders), sidebar elements are also flat.\
Configuration is possible:
* [Docusaurus Docs: Docs Sidebar][docusaurus_docs_docs_sidebar]
* [Docusaurus Docs: blog Sidebar][docusaurus_docs_blog_sidebar]

### BasePath configuration

An important detail is the proper configuration of your `url` and `baseUrl`. Without this step, browsers won't be able to find styles and assets and run into 404 errors:
* [Docusaurus Docs: Deployment][docusaurus_docs_deployment]
* [ZubeApp Docusaurus Deployment PR][gitlab_zube_app_docusaurus_pull_request]


## Local Search through LunrJS

There's two options for implementing site searches
* Online / cloud based
* Local / offline

[Docusaurus recommends][docusaurus_docs_search], and officially supports, a proprietary cloud service called [Algolia DocSearch][algoia_docsearch]. It is a cloud service that periodically (every 24 hours) crawls through ones site and builds up a search index. This index can then be used to search for contents using Algolia APIs.

Offline alternatives exist however such as **[LunrJS][lunrjs_web]**.\
It works similar to algolia in that it builds a search index. The index is built at compile-time and provided to the user at runtime to be downloaded and query against. Multiple versions of docusaurus lunr search are available.
* https://github.com/lelouch77/docusaurus-lunr-search
* https://github.com/easyops-cn/docusaurus-search-local
    * Picked this one because
    * It is written in Typescript
    * Is simple to setup
    * Highlighting search results on target pages
    * Provides a "show all search results" option
    * Adds a dedicated realtime search screen

## Gitlab Pages

Now that we have a docusaurus page up and running, we'll have to host it somewhere. Gitlab provides a service called [Gitlab Pages][gitlab_pages_doc]. It lts you publish static content directly from a gitlab repository.
* Use any static site generator (Gatsby, Jekyll, Hugo, Middleman, Harp, Hexo, Brunch, Docusaurus, ...) or plain HTML
* Create websites for projects, groups or user account
* Host the site for free
* Connect your custom domains and TLS certificates
* Attribute any license to your content
* No dynamic server-side processing is supported (.php, .asp)

### Deployment

To deploy a gitlab page one has to build and deploy the site through Gitlab CI/CD pipelines.\
Thankfully there's an official example on how to do that:
* [Docusaurus Gitlab Pages Example][gitlab_pages_example_docusaurus]
* [ZubeApp Docusaurus Deployment PR][gitlab_zube_app_docusaurus_pull_request]

## Gitlab CI/CD

### Using variables

One can use variables and interpolation to create new variables.
```yml
variables:
  DOCUSAURUS_PAGE_DIR: $CI_PROJECT_DIR/docs/zube
  DOCUSAURUS_BUILD_DIR: $DOCUSAURUS_PAGE_DIR/build
  DOCUSAURUS_GITLAB_PAGES_ARTIFACT_DIR: $CI_PROJECT_DIR/public
```

### Environment variables

If you aren't sure what variables gitlab has to offer, you can either [visit the docs][gitlab_cicd_variables] or export the variables in your pipeline:
```yml
# Pipeline for debugging purposes only
list:
  stage: build
  script:
    # Listing all environment variables is a security risk! Use with caution!
    # https://docs.gitlab.com/ee/ci/variables/index.html#list-all-environment-variables
    - export
```

### before_script

A pipeline can contain multiple tasks. Sometimes there's setup steps to perform for each and every one of them. To not having to rely on copy/paste (feat copy pasta 👀!) there's a dedicated directive called [`before_script`][gitlab_cicd_before_script].

> Use before_script to define an array of commands that should run before each job’s script commands, but after artifacts are restored. 
> You can use it only as part of a job or in the default section.



[docusaurus_docs_search]: https://docusaurus.io/docs/search
[docusaurus_docs_deployment]: https://docusaurus.io/docs/deployment
[docusaurus_docs_docs_sidebar]: https://docusaurus.io/docs/sidebar
[docusaurus_docs_blog_sidebar]: https://docusaurus.io/docs/blog#blog-sidebar

[gitlab_pages_doc]: https://docs.gitlab.com/ee/user/project/pages/
[gitlab_pages_example_docusaurus]: https://gitlab.com/pages/docusaurus
[gitlab_cicd_variables]: https://docs.gitlab.com/ee/ci/variables/index.html#protect-a-cicd-variable
[gitlab_cicd_before_script]: https://docs.gitlab.com/ee/ci/yaml/#before_script
[gitlab_zube_app_docusaurus_pull_request]: https://gitlab.com/ohsnaparts/osasoftworks/zube/zube-app/-/merge_requests/22

[osaportfolio_page]: https://ohsnaparts.gitlab.io/osasoftworks/portfolio/osaportfolio/
[algoia_docsearch]: https://www.algolia.com/
[lunrjs_web]: https://lunrjs.com/