import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'HitPaw MangaDex Manager',
  description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++.',
  lang: 'en-US',
  // Project site at https://hit-paw.github.io/HitPaw-MangaDex-Manager/ needs '/HitPaw-MangaDex-Manager/' — for custom domain (e.g., docs.hitpaw.dev) set to '/'
  base: '/HitPaw-MangaDex-Manager/',
  head: [
    ['link', { rel: 'icon', href: '/hitpaw.ico' }],
    ['meta', { name: 'theme-color', content: '#0ea5e9' }],
    ['meta', { property: 'og:title', content: 'HitPaw MangaDex Manager' }],
    ['meta', { property: 'og:description', content: 'Browse, filter, and export your MangaDex library. Built with Qt6/C++.' }]
  ],
  ignoreDeadLinks: true,
  themeConfig: {
    logo: '/icon_64.png',
    siteTitle: 'HitPaw Manager',
    nav: [
      { text: 'Guide', link: '/getting-started' },
      { text: 'Building', link: '/building' },
      { text: 'Releases', link: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases' },
      { text: 'Discord', link: 'https://discord.gg/z6yYYpcYYc' }
    ],
    sidebar: [
      {
        text: 'Introduction',
        items: [
          { text: 'What is HitPaw?', link: '/' },
          { text: 'Getting Started', link: '/getting-started' },
          { text: 'Download', link: '/download' }
        ]
      },
      {
        text: 'Guide',
        items: [
          { text: 'Building from Source', link: '/building' },
          { text: 'Export Formats', link: '/export' },
          { text: 'Security', link: '/security' }
        ]
      },
      {
        text: 'Community',
        items: [
          { text: 'Contributing', link: '/contributing' },
          { text: 'Changelog', link: '/changelog' },
          { text: 'Code of Conduct', link: '/code-of-conduct' }
        ]
      }
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager' },
      { icon: 'discord', link: 'https://discord.gg/z6yYYpcYYc' }
    ],
    footer: {
      message: 'Not affiliated with or endorsed by MangaDex. Released under MIT.',
      copyright: 'Copyright © 2026 Hit-Paw'
    },
    search: { provider: 'local' },
    editLink: {
      pattern: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/edit/main/docs/:path',
      text: 'Edit this page on GitHub'
    }
  },
  vite: {
    // Fix for Windows watch
  }
})
