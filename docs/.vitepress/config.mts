import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'HitPaw MangaDex Manager',
  titleTemplate: ':title — HitPaw MangaDex Manager',
  description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++. Fast, AMOLED, 100% local.',
  lang: 'en-US',
  // Project site at https://hit-paw.github.io/HitPaw-MangaDex-Manager/ needs '/HitPaw-MangaDex-Manager/' — for custom domain (e.g., docs.hitpaw.dev) set to '/'
  base: '/HitPaw-MangaDex-Manager/',
  // Brand is black + orange (AMOLED) — force dark, no toggle, no light-mode flash
  appearance: 'force-dark',
  sitemap: {
    hostname: 'https://hit-paw.github.io'
  },
  // Fix sitemap base — VitePress 1.6 does not auto-prefix base in sitemap when hostname is apex
  async buildEnd(siteConfig) {
    try {
      const { readFile, writeFile } = await import('node:fs/promises')
      const { join } = await import('node:path')
      const p = join(siteConfig.outDir, 'sitemap.xml')
      let xml = await readFile(p, 'utf-8')
      const before = xml
      xml = xml.replaceAll('https://hit-paw.github.io/', 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/')
      // homepage case without trailing slash already covered, but ensure single
      xml = xml.replaceAll('HitPaw-MangaDex-Manager//', 'HitPaw-MangaDex-Manager/')
      if (xml !== before) await writeFile(p, xml, 'utf-8')
    } catch {}
  },
  cleanUrls: false,
  lastUpdated: true,
  ignoreDeadLinks: [/^https:\/\/discord\.gg/, /^https:\/\/github\.com\/Hit-Paw\/HitPaw-MangaDex-Manager\/releases/],
  markdown: {
    theme: { light: 'github-light', dark: 'github-dark' },
    lineNumbers: false,
    image: { lazyLoading: true },
    headers: { level: [2, 3] }
  },
  head: [
    // Favicon — base-prefixed for GitHub Pages project site
    ['link', { rel: 'icon', type: 'image/x-icon', href: '/HitPaw-MangaDex-Manager/hitpaw.ico' }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '16x16', href: '/HitPaw-MangaDex-Manager/icon_16.png' }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '32x32', href: '/HitPaw-MangaDex-Manager/icon_32.png' }],
    ['link', { rel: 'apple-touch-icon', sizes: '180x180', href: '/HitPaw-MangaDex-Manager/icon_256.png' }],
    ['link', { rel: 'manifest', href: '/HitPaw-MangaDex-Manager/manifest.webmanifest' }],
    // Theme
    ['meta', { name: 'theme-color', content: '#ff6a00', media: '(prefers-color-scheme: light)' }],
    ['meta', { name: 'theme-color', content: '#0a0a0a', media: '(prefers-color-scheme: dark)' }],
    ['meta', { name: 'color-scheme', content: 'dark light' }],
    // SEO
    ['meta', { name: 'author', content: 'Hit-Paw' }],
    ['meta', { name: 'keywords', content: 'MangaDex, manga manager, manga library, MAL, AniList, Kitsu, MangaBaka, Anime-Planet, Qt6, HitPaw' }],
    ['meta', { name: 'robots', content: 'index, follow' }],
    ['meta', { property: 'og:type', content: 'website' }],
    ['meta', { property: 'og:site_name', content: 'HitPaw MangaDex Manager' }],
    ['meta', { property: 'og:title', content: 'HitPaw MangaDex Manager — Browse, filter, export your MangaDex library' }],
    ['meta', { property: 'og:description', content: 'Desktop app built with Qt6/C++ — 5-column AMOLED grid, Show All (N), offline exports to MAL/AniList/MangaBaka/Kitsu. 100% local & secure.' }],
    ['meta', { property: 'og:url', content: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/' }],
    ['meta', { property: 'og:image', content: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/og-image.png' }],
    ['meta', { property: 'og:image:width', content: '1200' }],
    ['meta', { property: 'og:image:height', content: '630' }],
    ['meta', { property: 'og:image:alt', content: 'HitPaw MangaDex Manager — Desktop app preview' }],
    ['meta', { property: 'og:image:type', content: 'image/png' }],
    ['meta', { property: 'og:locale', content: 'en_US' }],
    // Twitter
    ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
    ['meta', { name: 'twitter:title', content: 'HitPaw MangaDex Manager' }],
    ['meta', { name: 'twitter:description', content: 'Browse, filter, and export your MangaDex library. Built with Qt6/C++ — dark AMOLED, offline exports.' }],
    ['meta', { name: 'twitter:image', content: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/og-image.png' }],
    ['meta', { name: 'twitter:image:alt', content: 'HitPaw MangaDex Manager preview' }],
    // Preconnects / perf
    ['link', { rel: 'preconnect', href: 'https://github.com' }],
    ['link', { rel: 'dns-prefetch', href: 'https://github.com' }],
    // JSON-LD SoftwareApplication
    ['script', { type: 'application/ld+json' }, JSON.stringify({
      '@context': 'https://schema.org',
      '@type': 'SoftwareApplication',
      name: 'HitPaw MangaDex Manager',
      applicationCategory: 'UtilitiesApplication',
      operatingSystem: 'Windows, macOS, Linux',
      description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++.',
      url: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/',
      downloadUrl: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest',
      author: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
      offers: { '@type': 'Offer', price: '0', priceCurrency: 'USD', availability: 'https://schema.org/InStock' },
      softwareVersion: '3.4.6',
      license: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/LICENSE',
      screenshot: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/preview-1.png',
      aggregateRating: { '@type': 'AggregateRating', ratingValue: '5', ratingCount: '1' }
    })]
  ],
  transformHead({ pageData }) {
    const url = `https://hit-paw.github.io/HitPaw-MangaDex-Manager/${pageData.relativePath.replace(/index\.md$/, '').replace(/\.md$/, '.html')}`
    const canonical = pageData.relativePath === 'index.md' ? 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/' : url
    return [
      ['link', { rel: 'canonical', href: canonical }],
      ['meta', { property: 'og:url', content: canonical }]
    ]
  },
  themeConfig: {
    logo: '/icon_64.png',
    siteTitle: 'HitPaw MangaDex Manager',
    outline: { level: [2, 3], label: 'On this page' },
    lastUpdated: { text: 'Last updated', formatOptions: { dateStyle: 'medium' } },
    docFooter: { prev: 'Previous', next: 'Next' },
    darkModeSwitchLabel: 'Appearance',
    lightModeSwitchTitle: 'Switch to light theme',
    darkModeSwitchTitle: 'Switch to dark theme',
    sidebarMenuLabel: 'Menu',
    returnToTopLabel: 'Return to top',
    nav: [
      { text: 'Guide', link: '/getting-started' },
      { text: 'Building', link: '/building' },
      { text: 'Releases', link: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases' },
      { text: 'Discord', link:
