import DefaultTheme from 'vitepress/theme'
import { h, onMounted, watch, nextTick } from 'vue'
import { useRoute } from 'vitepress'
import './custom.css'
import './lightbox-viewport.css'
import NotFoundPage from './NotFoundPage.vue'
import AnnounceBar from './components/AnnounceBar.vue'
import DocMeta from './components/DocMeta.vue'
import DocFeedback from './components/DocFeedback.vue'

export default {
  extends: DefaultTheme,
  // Custom branded 404 — VitePress's router renders its own default NotFound
  // component for unmatched routes (404.md content is never shown in the SPA),
  // so we replace it through the layout's `not-found` slot.
  // Pro layer slots: announcement bar, doc meta row
  // (section · reading time · updated · copy page), page feedback widget.
  Layout: () => h(DefaultTheme.Layout, null, {
    'not-found': () => h(NotFoundPage),
    'layout-top': () => h(AnnounceBar),
    'doc-before': () => h(DocMeta),
    'doc-after': () => h(DocFeedback)
  }),
  setup(props: any, ctx: any) {
    // Preserve DefaultTheme setup (search, etc.)
    let parentResult: any = undefined
    try {
      const maybeSetup = (DefaultTheme as any).setup
      if (typeof maybeSetup === 'function') parentResult = maybeSetup(props, ctx)
    } catch {}

    const route = useRoute()

  // Shared reveal selector — must mirror the CSS reveal block in custom.css
  // (v4 "Editorial Ink": subtle 200ms fades on cards/rules/tables only)
  const REVEAL_SELECTOR = [
    '.stat-card',
    '.why-card',
    '.community-card',
    '.quick-card',
    '.faq-strip details',
    '.preview-grid .preview-card',
    '.section-head',
    '.vs-table',
    '.cta-banner',
    '.vp-doc table',
    '.vp-doc blockquote',
    '.vp-doc div[class*="language-"]',
    '.vp-doc .custom-block',
    '.vp-doc details'
  ].join(', ')

    const prefersReducedMotion = () =>
      typeof window.matchMedia === 'function' &&
      window.matchMedia('(prefers-reduced-motion: reduce)').matches

    // v4: no cursor spotlight, no count-ups — motion is intentionally subtle.

    // ============================================================
    // PRO LAYER — global one-time features
    //   1. Reading progress bar (docs pages only)
    //   2. Back-to-top button
    //   3. "/" keyboard shortcut → local search
    //   4. FAQ live filter (mounts into #faq-filter-root on faq.md)
    // ============================================================

    const isEditable = (el: EventTarget | null) => {
      if (!(el instanceof HTMLElement)) return false
      return el.isContentEditable ||
        el.tagName === 'INPUT' || el.tagName === 'TEXTAREA' || el.tagName === 'SELECT'
    }

    const setupProgressBar = () => {
      let bar = document.getElementById('hp-progress') as HTMLElement | null
      if (!bar) {
        bar = document.createElement('div')
        bar.id = 'hp-progress'
        bar.setAttribute('aria-hidden', 'true')
        const fill = document.createElement('div')
        fill.className = 'hp-progress-fill'
        bar.appendChild(fill)
        document.body.appendChild(bar)
      }
    }

    const updateProgress = () => {
      const bar = document.getElementById('hp-progress')
      if (!bar) return
      const doc = document.querySelector('.vp-doc')
      const max = document.documentElement.scrollHeight - window.innerHeight
      const active = !!doc && max > 80
      if (!active) { bar.classList.remove('show'); return }
      const pct = Math.min(1, Math.max(0, window.scrollY / max))
      const fill = bar.firstElementChild as HTMLElement | null
      if (fill) fill.style.transform = `scaleX(${pct})`
      bar.classList.toggle('show', window.scrollY > 40)
    }

    const setupBackToTop = () => {
      let btn = document.getElementById('hp-top') as HTMLButtonElement | null
      if (!btn) {
        btn = document.createElement('button')
        btn.id = 'hp-top'
        btn.type = 'button'
        btn.className = 'hp-top'
        btn.setAttribute('aria-label', 'Back to top')
        btn.innerHTML = '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="m18 15-6-6-6 6"/></svg>'
        btn.addEventListener('click', () => {
          window.scrollTo({ top: 0, behavior: prefersReducedMotion() ? 'auto' : 'smooth' })
        })
        document.body.appendChild(btn)
      }
    }

    const updateBackToTop = () => {
      const btn = document.getElementById('hp-top')
      if (!btn) return
      const show = window.scrollY > 560
      btn.classList.toggle('show', show)
      // Keep clear of the lightbox when it is open
      const lbOpen = document.getElementById('lightbox')?.classList.contains('open')
      if (lbOpen) btn.classList.remove('show')
    }

    let scrollWired = false
    const wireScroll = () => {
      if (scrollWired) return
      scrollWired = true
      let ticking = false
      const onScroll = () => {
        if (ticking) return
        ticking = true
        requestAnimationFrame(() => {
          updateProgress()
          updateBackToTop()
          ticking = false
        })
      }
      window.addEventListener('scroll', onScroll, { passive: true })
      window.addEventListener('resize', onScroll, { passive: true })
      onScroll()
    }

    const onSearchKey = (e: KeyboardEvent) => {
      if (e.key !== '/' || e.metaKey || e.ctrlKey || e.altKey) return
      if (isEditable(e.target)) return
      // Don't hijack when an overlay is open
      if (document.getElementById('lightbox')?.classList.contains('open')) return
      if (document.querySelector('.VPLocalSearchBox')) return
      const btn = document.querySelector('.VPNavBarSearch button') as HTMLElement | null
      if (btn && btn.offsetParent !== null) {
        e.preventDefault()
        btn.click()
      }
    }

    // ---------------- FAQ live filter ----------------
    const setupFaqFilter = () => {
      const root = document.getElementById('faq-filter-root')
      if (!root || root.dataset.init === '1') return
      const doc = root.closest('.vp-doc') as HTMLElement | null
      if (!doc) return
      root.dataset.init = '1'
      root.innerHTML = ''

      const wrap = document.createElement('div')
      wrap.className = 'faq-filter'
      wrap.setAttribute('role', 'search')

      const input = document.createElement('input')
      input.type = 'search'
      input.className = 'faq-filter-input'
      input.placeholder = 'Filter questions…'
      input.setAttribute('aria-label', 'Filter frequently asked questions')

      const count = document.createElement('span')
      count.className = 'faq-filter-count'
      count.setAttribute('aria-live', 'polite')

      const clear = document.createElement('button')
      clear.type = 'button'
      clear.className = 'faq-filter-clear'
      clear.setAttribute('aria-label', 'Clear filter')
      clear.textContent = '×'
      clear.style.display = 'none'

      wrap.appendChild(input)
      wrap.appendChild(clear)
      wrap.appendChild(count)
      root.appendChild(wrap)

      const rows = Array.from(doc.querySelectorAll('details'))
      const sections = Array.from(doc.querySelectorAll('h2'))
      const total = rows.length
      const setCount = (shown: number) => {
        count.textContent = input.value.trim()
          ? `${shown} / ${total} shown`
          : ''
        clear.style.display = input.value ? 'flex' : 'none'
      }

      const apply = () => {
        const q = input.value.trim().toLowerCase()
        let shown = 0
        rows.forEach((d) => {
          const text = (d.textContent || '').toLowerCase()
          const match = !q || text.includes(q)
          d.classList.toggle('faq-hidden', !match)
          if (match) shown++
        })
        // Hide section headings whose rows are all filtered out
        sections.forEach((h) => {
          let any = false
          let node: Element | null = h.nextElementSibling
          while (node && node.tagName !== 'H2') {
            if (node.tagName === 'DETAILS' && !node.classList.contains('faq-hidden')) { any = true; break }
            node = node.nextElementSibling
          }
          h.classList.toggle('faq-hidden', q !== '' && !any)
        })
        setCount(shown)
      }

      input.addEventListener('input', apply)
      clear.addEventListener('click', () => { input.value = ''; apply(); input.focus() })
      apply()
    }

    // ---------------- Code copy button: strip native tooltip ----------------
    // VitePress bakes `title="Copy Code"` into every fence button at build
    // time (config fallback re-adds it if set falsy), which pops the ugly
    // OS-native tooltip on hover. Remove it client-side; `aria-label` keeps
    // the button accessible. A MutationObserver covers SPA re-renders and
    // VitePress's own attribute writes; rAF-debounced.
    const stripCopyTooltips = () => {
      document
        .querySelectorAll('div[class*="language-"] > button.copy[title]')
        .forEach((b) => {
          b.removeAttribute('title')
          if (!b.getAttribute('aria-label')) b.setAttribute('aria-label', 'Copy code')
        })
    }

    const setupCopyTooltipStrip = () => {
      if (copyTitleMO) return
      stripCopyTooltips()
      let queued = false
      copyTitleMO = new MutationObserver(() => {
        if (queued) return
        queued = true
        requestAnimationFrame(() => { queued = false; stripCopyTooltips() })
      })
      copyTitleMO.observe(document.body, {
        childList: true,
        subtree: true,
        attributeFilter: ['title']
      })
    }

    const setupProFeatures = () => {
      setupProgressBar()
      setupBackToTop()
      wireScroll()
      requestAnimationFrame(() => { updateProgress(); updateBackToTop() })
      setupFaqFilter()
      setupCopyTooltipStrip()
    }

    const onReveal = (el: Element) => {
      el.classList.add('in-view')
    }

    // Mark JS enabled for CSS no-js fallback
    const markJS = () => {
      try { document.documentElement.classList.add('js') } catch {}
    }

    // Single reusable observer — avoids leaking one IntersectionObserver per SPA
    // navigation; also skips elements already revealed so re-runs are idempotent.
    let revealIO: IntersectionObserver | null = null
    let copyTitleMO: MutationObserver | null = null

    const observe = () => {
      // Respect reduced motion — reveal instantly
      if (prefersReducedMotion()) {
        document.querySelectorAll(REVEAL_SELECTOR)
          .forEach((el) => el.classList.add('in-view'))
        return
      }
      const els = Array.from(document.querySelectorAll(REVEAL_SELECTOR))
        .filter((el) => !el.classList.contains('in-view'))
      if (!els.length) return
      if (!('IntersectionObserver' in window)) {
        els.forEach((el) => el.classList.add('in-view'))
        return
      }
      if (revealIO) revealIO.disconnect()
      revealIO = new IntersectionObserver(
        (entries) => {
          entries.forEach((e) => {
            if (e.isIntersecting) {
              onReveal(e.target)
              revealIO?.unobserve(e.target)
            }
          })
        },
        { threshold: 0.12, rootMargin: '0px 0px -8% 0px' }
      )
      els.forEach((el) => revealIO?.observe(el))
    }

    // Lightbox — a11y dialog, focus trap, keyboard
    let cleanupLightbox: (() => void) | null = null

    const setupLightbox = () => {
      if (cleanupLightbox) { cleanupLightbox(); cleanupLightbox = null }
      let lb = document.getElementById('lightbox') as HTMLElement | null
      // Auto-create lightbox if not present (no markdown needed)
      if (!lb) {
        lb = document.createElement('div')
        lb.id = 'lightbox'
        lb.className = 'lightbox'
        lb.setAttribute('aria-hidden', 'true')
        const innerImg = document.createElement('img')
        innerImg.alt = ''
        innerImg.decoding = 'async'
        lb.appendChild(innerImg)
        const cap = document.createElement('div')
        cap.className = 'lightbox-caption'
        cap.setAttribute('aria-live', 'polite')
        lb.appendChild(cap)
        const btnEl = document.createElement('button')
        btnEl.type = 'button'
        btnEl.className = 'lightbox-close'
        btnEl.setAttribute('aria-label', 'Close preview (Escape)')
        btnEl.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" aria-hidden="true"><path d="M18 6L6 18"/><path d="M6 6l12 12"/></svg>'
        lb.appendChild(btnEl)
        document.body.appendChild(lb)
      }

      // Ensure lightbox is direct child of body for fixed viewport positioning
      if (lb.parentElement !== document.body) {
        document.body.appendChild(lb)
      }

      lb.setAttribute('role', 'dialog')
      lb.setAttribute('aria-modal', 'true')
      lb.setAttribute('aria-label', 'Image preview — press Escape to close')

      let img = lb.querySelector('img') as HTMLImageElement | null
      let caption = lb.querySelector('.lightbox-caption') as HTMLElement | null
      let btn = lb.querySelector('.lightbox-close') as HTMLButtonElement | null
      // Safety: ensure essentials exist
      if (!btn) {
        btn = document.createElement('button')
        btn.type = 'button'
        btn.className = 'lightbox-close'
        btn.setAttribute('aria-label', 'Close preview')
        btn.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M18 6L6 18"/><path d="M6 6l12 12"/></svg>'
        lb.appendChild(btn)
      }
      if (!caption) {
        caption = document.createElement('div')
        caption.className = 'lightbox-caption'
        lb.appendChild(caption)
      }
      if (!img) {
        img = document.createElement('img')
        lb.prepend(img)
      }

      let lastFocus: HTMLElement | null = null
      const controllers: AbortController[] = []
      const ac = () => { const c = new AbortController(); controllers.push(c); return c.signal }

      const open = (src: string, alt: string) => {
        if (!img) return
        lastFocus = document.activeElement as HTMLElement | null
        img.src = src
        img.alt = alt
        if (caption) caption.textContent = alt
        lb.classList.add('open')
        lb.setAttribute('aria-hidden', 'false')
        document.body.style.overflow = 'hidden'
        requestAnimationFrame(() => btn && btn.focus())
      }
      const close = () => {
        lb.classList.remove('open')
        lb.setAttribute('aria-hidden', 'true')
        document.body.style.overflow = ''
        if (lastFocus && typeof lastFocus.focus === 'function') {
          try { lastFocus.focus() } catch {}
        }
      }

      lb.addEventListener('click', (e) => { if (e.target === lb) close() }, { signal: ac() })
      if (btn) btn.addEventListener('click', close, { signal: ac() })
      if (img) img.addEventListener('click', close, { signal: ac() })

      const onKey = (e: KeyboardEvent) => {
        if (!lb.classList.contains('open')) return
        if (e.key === 'Escape') { e.preventDefault(); close(); return }
        // Complete focus trap: the close button is the only focusable element in
        // the dialog, so keep focus on it for both Tab directions and refocus if
        // focus somehow escapes the overlay.
        if (e.key === 'Tab' && btn) {
          const inside = lb.contains(document.activeElement)
          if (!inside || document.activeElement === btn) {
            e.preventDefault()
            btn.focus()
          }
        }
      }
      document.addEventListener('keydown', onKey, { signal: ac() })

      // Support both plain <img> and <picture><img> with WebP
      const previews = document.querySelectorAll<HTMLImageElement>('.preview-grid img')
      previews.forEach((el) => {
        // Prefer parent <picture> source if present
        const picture = el.closest('picture')
        const getBestSrc = () => {
          // currentSrc already resolves <source> WebP when supported
          try {
            const cs = (el as any).currentSrc
            if (cs) return cs as string
          } catch {}
          // Fallback: check <source srcset> manually
          if (picture) {
            const srcEl = picture.querySelector('source[type="image/webp"]') as HTMLSourceElement | null
            if (srcEl && srcEl.srcset) return srcEl.srcset.split(',')[0].trim().split(' ')[0]
          }
          return el.src
        }
        el.setAttribute('tabindex', '0')
        el.setAttribute('role', 'button')
        el.setAttribute('aria-label', `Enlarge preview: ${el.alt || 'screenshot'}`)
        el.style.cursor = 'zoom-in'
        // Perf: eager featured, lazy grid already set via HTML; add loading hint if missing
        if (!el.getAttribute('loading')) el.setAttribute('loading', 'lazy')
        if (!el.getAttribute('decoding')) el.setAttribute('decoding', 'async')
        const handler = () => {
          open(getBestSrc(), el.alt || '')
        }
        el.addEventListener('click', handler, { signal: ac() })
        el.addEventListener('keydown', (e: KeyboardEvent) => {
          if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); handler() }
        }, { signal: ac() })
        // Also make whole picture card clickable (better hit area on mobile)
        const card = el.closest('.preview-card') as HTMLElement | null
        if (card && card !== el) {
          // Ensure card does not trap inner img handler double-fire
          card.style.cursor = 'zoom-in'
          card.addEventListener('click', (e) => {
            if (e.target === el) return
            handler()
          }, { signal: ac() })
        }
      })

      cleanupLightbox = () => {
        controllers.forEach((c) => c.abort())
        // Fully reset overlay state (e.g. SPA navigation happened while open)
        if (lb.classList.contains('open')) {
          lb.classList.remove('open')
          lb.setAttribute('aria-hidden', 'true')
        }
        document.body.style.overflow = ''
      }
    }

    onMounted(() => {
      markJS()
      try {
        if ('scrollRestoration' in history) (history as any).scrollRestoration = 'manual'
        if (!window.location.hash) {
          window.scrollTo(0, 0)
          requestAnimationFrame(() => window.scrollTo(0, 0))
          setTimeout(() => window.scrollTo(0, 0), 50)
        }
      } catch {}
      nextTick(() => { observe(); setupLightbox(); setupProFeatures(); setTimeout(observe, 250) })
      // "/" focuses search (document-level, wired once)
      document.addEventListener('keydown', onSearchKey)
    })
    watch(() => route.path, () => nextTick(() => {
      try { if (!window.location.hash) window.scrollTo(0, 0) } catch {}
      observe(); setupLightbox(); setupProFeatures()
      // Safety net: re-scan shortly after navigation in case async page content
      // (lazy chunks, dynamically mounted blocks) renders after nextTick.
      setTimeout(observe, 250)
    }))

    return parentResult
  }
}
