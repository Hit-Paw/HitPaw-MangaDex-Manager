import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'HitPaw MangaDex Manager',
  titleTemplate: ':title — HitPaw MangaDex Manager',
  description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++. Fast, AMOLED, 100% local.',
  lang: 'en-US',
  // Project site at https://hit-paw.github.io/HitPaw-MangaDex-Manager/ needs '/HitPaw-MangaDex-Manager/' — for custom domain (e.g., docs.hitpaw.dev) set to '/'
  base: '/HitPaw-MangaDex-Manager/',
  appearance: 'force-dark',
  sitemap: {
    // Include base in hostname — VitePress 1.6 does not auto-prefix base when hostname is apex
    hostname: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager'
  },
  // Robust sitemap base fix — VitePress 1.6 project-page quirk (polls until complete)
  async buildEnd(siteConfig) {
    const { readFile, writeFile, access } = await import('node:fs/promises')
    const { join } = await import('node:path')
    const candidates = [
      siteConfig?.outDir ? join(siteConfig.outDir, 'sitemap.xml') : null,
      join(process.cwd(), 'docs', '.vitepress', 'dist', 'sitemap.xml'),
      join(process.cwd(), 'docs', 'dist', 'sitemap.xml'),
      join(process.cwd(), 'dist', 'sitemap.xml'),
    ].filter(Boolean) as string[]
    let p: string | null = null
    for (const cand of candidates) {
      try { await access(cand); p = cand; break } catch {}
    }
    if (!p) {
      await new Promise(r => setTimeout(r, 500))
      for (const cand of candidates) {
        try { await access(cand); p = cand; break } catch {}
      }
    }
    if (!p) return
    try {
      let attempts = 0
      let xmlPre = ''
      while (attempts < 12) {
        try {
          xmlPre = await readFile(p, 'utf-8')
          if (xmlPre.includes('</urlset>') && xmlPre.length > 900) break
        } catch {}
        await new Promise(r => setTimeout(r, 250))
        attempts++
      }
      if (!xmlPre.includes('</urlset>')) return
      let xml = xmlPre
      const before = xml
      const apex = 'https://hit-paw.github.io/'
      const base = 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/'
      const baseNoSlash = 'https://hit-paw.github.io/HitPaw-MangaDex-Manager'
      const marker = '__HITPAW_BASE_SLASH__'
      const markerNoSlash = '__HITPAW_BASE_NOSLASH__'
      xml = xml.replaceAll(base, marker)
      xml = xml.replaceAll(baseNoSlash, markerNoSlash)
      xml = xml.replaceAll(apex, base)
      xml = xml.replaceAll(marker, base)
      xml = xml.replaceAll(markerNoSlash, base)
      xml = xml.replaceAll('HitPaw-MangaDex-Manager//', 'HitPaw-MangaDex-Manager/')
      xml = xml.replaceAll('HitPaw-MangaDex-Manager/HitPaw-MangaDex-Manager', 'HitPaw-MangaDex-Manager')
      xml = xml.replaceAll('https://hit-paw.github.io/HitPaw-MangaDex-Manager</loc>', 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/</loc>')
      if (xml !== before) await writeFile(p, xml, 'utf-8')
    } catch {}
  },
  cleanUrls: false,
  lastUpdated: true,
  ignoreDeadLinks: [/^https:\/\/discord\.gg/, /^https:\/\/github\.com\/Hit-Paw\/HitPaw-MangaDex-Manager\/releases/, /mermaid/, /^http:\/\/localhost:/, /^https:\/\/mangadex\.org/],
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
    ['meta', { name: 'format-detection', content: 'telephone=no' }],
    // SEO — core
    ['meta', { name: 'author', content: 'Hit-Paw' }],
    ['meta', { name: 'keywords', content: 'MangaDex, manga manager, manga library, MAL, AniList, Kitsu, MangaBaka, Anime-Planet, Qt6, HitPaw' }],
    ['meta', { name: 'robots', content: 'index, follow, max-image-preview:large' }],
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
    ['meta', { name: 'twitter:site', content: '@HitPaw' }],
    ['meta', { name: 'twitter:creator', content: '@HitPaw' }],
    ['meta', { name: 'twitter:title', content: 'HitPaw MangaDex Manager' }],
    ['meta', { name: 'twitter:description', content: 'Browse, filter, and export your MangaDex library. Built with Qt6/C++ — dark AMOLED, offline exports.' }],
    ['meta', { name: 'twitter:image', content: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/og-image.png' }],
    ['meta', { name: 'twitter:image:alt', content: 'HitPaw MangaDex Manager preview' }],
    // Perf — critical preloads
    ['link', { rel: 'preload', as: 'image', href: '/HitPaw-MangaDex-Manager/preview-1.webp', type: 'image/webp', fetchpriority: 'high' }],
    ['link', { rel: 'preload', as: 'image', href: '/HitPaw-MangaDex-Manager/icon_256.png', type: 'image/png' }],
    ['link', { rel: 'preconnect', href: 'https://github.com' }],
    ['link', { rel: 'dns-prefetch', href: 'https://github.com' }],
    ['link', { rel: 'preconnect', href: 'https://api.mangadex.org' }],
    ['link', { rel: 'dns-prefetch', href: 'https://api.mangadex.org' }],
    ['link', { rel: 'dns-prefetch', href: 'https://uploads.mangadex.org' }],
    // JSON-LD SoftwareApplication + WebSite + Organization — primary graph
    ['script', { type: 'application/ld+json' }, JSON.stringify({
      '@context': 'https://schema.org',
      '@graph': [
        {
          '@type': 'SoftwareApplication',
          name: 'HitPaw MangaDex Manager',
          applicationCategory: 'UtilitiesApplication',
          operatingSystem: 'Windows, macOS, Linux',
          description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++. Fast AMOLED grid, Show All (N), offline exports to MAL/AniList/MangaBaka/Kitsu. 100% local & secure.',
          url: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/',
          downloadUrl: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest',
          installUrl: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/download.html',
          author: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          publisher: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          offers: { '@type': 'Offer', price: '0', priceCurrency: 'USD', availability: 'https://schema.org/InStock' },
          softwareVersion: '3.5.0',
          releaseNotes: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/changelog.html',
          license: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/LICENSE',
          screenshot: [
            'https://hit-paw.github.io/HitPaw-MangaDex-Manager/preview-1.png',
            'https://hit-paw.github.io/HitPaw-MangaDex-Manager/preview-2.png',
            'https://hit-paw.github.io/HitPaw-MangaDex-Manager/preview-3.png',
            'https://hit-paw.github.io/HitPaw-MangaDex-Manager/preview-4.png'
          ],
          featureList: 'Library grid, Cover cache, Export CSV/JSON/MAL/MB/AP, Sync to MDList, Update check, Secure local storage',
          aggregateRating: { '@type': 'AggregateRating', ratingValue: '5', ratingCount: '1' }
        },
        {
          '@type': 'WebSite',
          name: 'HitPaw MangaDex Manager Docs',
          url: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/',
          publisher: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          inLanguage: 'en-US',
          potentialAction: {
            '@type': 'SearchAction',
            target: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/?q={search_term_string}',
            'query-input': 'required name=search_term_string'
          }
        },
        {
          '@type': 'Organization',
          name: 'Hit-Paw',
          url: 'https://github.com/Hit-Paw',
          logo: 'https://hit-paw.github.io/HitPaw-MangaDex-Manager/icon_512.png',
          sameAs: ['https://github.com/Hit-Paw/HitPaw-MangaDex-Manager', 'https://discord.gg/z6yYYpcYYc']
        }
      ]
    })]
  ],
  transformHead({ pageData }) {
    const baseUrl = 'https://hit-paw.github.io/HitPaw-MangaDex-Manager'
    const clean = pageData.relativePath.replace(/index\.md$/, '').replace(/\.md$/, '.html')
    const url = clean ? `${baseUrl}/${clean}` : `${baseUrl}/`
    const canonical = pageData.relativePath === 'index.md' ? `${baseUrl}/` : url
    const title = (pageData.frontmatter as any)?.title || pageData.title || 'HitPaw MangaDex Manager'
    const desc = (pageData.frontmatter as any)?.description || (pageData as any).description || 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++.'
    // Breadcrumb JSON-LD per page
    const parts = clean.replace(/\.html$/, '').split('/').filter(Boolean)
    const breadcrumb = parts.length ? {
      '@context': 'https://schema.org',
      '@type': 'BreadcrumbList',
      itemListElement: [
        { '@type': 'ListItem', position: 1, name: 'Home', item: `${baseUrl}/` },
        ...parts.map((p: string, i: number) => ({
          '@type': 'ListItem',
          position: i + 2,
          name: p.replace(/-/g, ' ').replace(/\b\w/g, (c: string) => c.toUpperCase()),
          item: `${baseUrl}/${parts.slice(0, i + 1).join('/')}.html`
        }))
      ]
    } : null
    const heads: any[] = [
      ['link', { rel: 'canonical', href: canonical }],
      ['meta', { property: 'og:url', content: canonical }],
      ['meta', { property: 'og:title', content: title }],
      ['meta', { property: 'og:description', content: desc }],
      ['meta', { name: 'twitter:title', content: title }],
      ['meta', { name: 'twitter:description', content: desc }]
    ]
    if (breadcrumb) heads.push(['script', { type: 'application/ld+json' }, JSON.stringify(breadcrumb)])
    return heads
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
          { text: 'Security', link: '/security' },
          { text: 'FAQ', link: '/faq' }
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
      message: 'Not affiliated with or endorsed by MangaDex. Released under <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/LICENSE" target="_blank">MIT</a>.',
      copyright: 'Copyright © 2026 Hit-Paw — Built with VitePress & Qt6'
    },
    search: { provider: 'local', options: { detailedView: true } },
    editLink: {
      pattern: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/edit/main/docs/:path',
      text: 'Edit this page on GitHub'
    }
  },
  vite: {
    // Perf: keep VitePress defaults + small tweaks — avoid aggressive manualChunks that breaks local search (mark.js)
    build: {
      chunkSizeWarningLimit: 800
    }
  }
})
