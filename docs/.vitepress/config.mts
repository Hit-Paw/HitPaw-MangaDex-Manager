import { defineConfig } from 'vitepress'

// Single source of truth for the deployed origin. Change this one line if the
// project ever moves off the GitHub Pages project path (e.g. to docs.hitpaw.dev) —
// previously this URL was hardcoded in ~15 places across JSON-LD/OG/sitemap config.
const SITE_URL = 'https://hit-paw.github.io/HitPaw-MangaDex-Manager'
const BASE_PATH = '/HitPaw-MangaDex-Manager/'

export default defineConfig({
  title: 'HitPaw MangaDex Manager',
  titleTemplate: ':title — HitPaw MangaDex Manager',
  description: 'Desktop app for browsing, filtering, and exporting your MangaDex library — Qt6/C++. Fast, AMOLED, 100% local.',
  lang: 'en-US',
  // Project site at https://hit-paw.github.io/HitPaw-MangaDex-Manager/ needs '/HitPaw-MangaDex-Manager/' — for custom domain (e.g., docs.hitpaw.dev) set BASE_PATH to '/'
  base: BASE_PATH,
  // Brand supports both themes — light (paper + orange) & dark (AMOLED).
  // `true` = follow system preference, manual toggle in the navbar, persisted.
  appearance: true,
  sitemap: {
    // Include base in hostname — VitePress 1.6 does not auto-prefix base when hostname is apex
    hostname: SITE_URL
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
      const base = SITE_URL + '/'
      const baseNoSlash = SITE_URL
      const marker = '__HITPAW_BASE_SLASH__'
      const markerNoSlash = '__HITPAW_BASE_NOSLASH__'
      xml = xml.replaceAll(base, marker)
      xml = xml.replaceAll(baseNoSlash, markerNoSlash)
      xml = xml.replaceAll(apex, base)
      xml = xml.replaceAll(marker, base)
      xml = xml.replaceAll(markerNoSlash, base)
      xml = xml.replaceAll('HitPaw-MangaDex-Manager//', 'HitPaw-MangaDex-Manager/')
      xml = xml.replaceAll('HitPaw-MangaDex-Manager/HitPaw-MangaDex-Manager', 'HitPaw-MangaDex-Manager')
      xml = xml.replaceAll(`${SITE_URL}</loc>`, `${SITE_URL}/</loc>`)
      // Fix: 404.html must not be indexed — drop it from the sitemap
      xml = xml.replace(/<url><loc>[^<]*\/404\.html<\/loc>[\s\S]*?<\/url>/g, '')
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
    headers: { level: [2, 3] },
    // Fix: VitePress rewrites markdown-syntax links with base + .html, and turns
    // raw-HTML <img src>/<source srcset> into module imports (also base-prefixed).
    // BUT raw-HTML <a href="/page"> is left as a literal string — no base prefix,
    // no .html — so those links 404 on the GitHub Pages project site. Rewrite
    // internal hrefs in html tokens here. Keep BASE in sync with `base` above.
    // Only touch href — src/srcset must stay literal for Vite's import pipeline.
    config(md) {
      const BASE = BASE_PATH
      const BASE_DIR = BASE.replace(/\/$/, '')
      const rewriteUrl = (url: string): string => {
        if (!url) return url
        if (url.startsWith('#')) return url
        if (/^[a-z][a-z0-9+.-]*:/i.test(url) || url.startsWith('//')) return url
        if (url.startsWith(BASE)) return url
        const parts = url.split(/([?#][\s\S]*)/, 2)
        const pathPart = parts[0]
        const suffix = parts[1] || ''
        let out: string
        if (pathPart === '/' || pathPart === '') out = BASE
        else if (/\.[a-z0-9]+$/i.test(pathPart)) out = BASE_DIR + pathPart
        else out = BASE_DIR + pathPart + '.html'
        return out + suffix
      }
      md.core.ruler.after('inline', 'hitpaw-base-raw-html-hrefs', (state: any) => {
        const visit = (tokens: any[]) => {
          for (const tok of tokens) {
            if (tok.type === 'html_block' || tok.type === 'html_inline') {
              tok.content = tok.content.replace(
                /(\shref=)"([^"]*)"/gi,
                (_m: string, attr: string, val: string) => attr + '"' + rewriteUrl(val) + '"'
              )
            }
            if (tok.children) visit(tok.children)
          }
        }
        visit(state.tokens)
      })
    }
  },
  head: [
    // Favicon — base-prefixed for GitHub Pages project site
    ['link', { rel: 'icon', type: 'image/x-icon', href: `${BASE_PATH}hitpaw.ico` }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '16x16', href: `${BASE_PATH}icon_16.png` }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '32x32', href: `${BASE_PATH}icon_32.png` }],
    ['link', { rel: 'apple-touch-icon', sizes: '180x180', href: `${BASE_PATH}icon_256.png` }],
    ['link', { rel: 'manifest', href: `${BASE_PATH}manifest.webmanifest` }],
    // Theme — matches page background per color mode (Editorial Ink paper/charcoal)
    ['meta', { name: 'theme-color', content: '#f6f2e9', media: '(prefers-color-scheme: light)' }],
    ['meta', { name: 'theme-color', content: '#151310', media: '(prefers-color-scheme: dark)' }],
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
    ['meta', { property: 'og:url', content: `${SITE_URL}/` }],
    ['meta', { property: 'og:image', content: `${SITE_URL}/og-image.png` }],
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
    ['meta', { name: 'twitter:image', content: `${SITE_URL}/og-image.png` }],
    ['meta', { name: 'twitter:image:alt', content: 'HitPaw MangaDex Manager preview' }],
    // Perf — critical preloads
    ['link', { rel: 'preload', as: 'image', href: `${BASE_PATH}preview-1.webp`, type: 'image/webp', fetchpriority: 'high' }],
    ['link', { rel: 'preload', as: 'image', href: `${BASE_PATH}icon_256.png`, type: 'image/png' }],
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
          url: `${SITE_URL}/`,
          downloadUrl: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest',
          installUrl: `${SITE_URL}/download.html`,
          author: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          publisher: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          offers: { '@type': 'Offer', price: '0', priceCurrency: 'USD', availability: 'https://schema.org/InStock' },
          softwareVersion: '3.5.0',
          releaseNotes: `${SITE_URL}/changelog.html`,
          license: 'https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/LICENSE',
          screenshot: [
            `${SITE_URL}/preview-1.png`,
            `${SITE_URL}/preview-2.png`,
            `${SITE_URL}/preview-3.png`,
            `${SITE_URL}/preview-4.png`
          ],
          featureList: 'Library grid, Cover cache, Export CSV/JSON/MAL/MB/AP, Sync to MDList, Update check, Secure local storage'
          // aggregateRating deliberately omitted — a single self-reported 5-star
          // rating is the pattern Google's structured-data guidelines flag as
          // low-trust. Re-add only once there's real review-count data behind it.
        },
        {
          '@type': 'WebSite',
          name: 'HitPaw MangaDex Manager Docs',
          url: `${SITE_URL}/`,
          publisher: { '@type': 'Organization', name: 'Hit-Paw', url: 'https://github.com/Hit-Paw' },
          inLanguage: 'en-US',
          potentialAction: {
            '@type': 'SearchAction',
            target: `${SITE_URL}/?q={search_term_string}`,
            'query-input': 'required name=search_term_string'
          }
        },
        {
          '@type': 'Organization',
          name: 'Hit-Paw',
          url: 'https://github.com/Hit-Paw',
          logo: `${SITE_URL}/icon_512.png`,
          sameAs: ['https://github.com/Hit-Paw/HitPaw-MangaDex-Manager', 'https://discord.gg/z6yYYpcYYc']
        }
      ]
    })]
  ],
  transformHead({ pageData }) {
    const baseUrl = SITE_URL
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
