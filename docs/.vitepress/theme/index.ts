import DefaultTheme from 'vitepress/theme'
import './custom.css'
import './lightbox-viewport.css'

import { onMounted, watch, nextTick } from 'vue'
import { useRoute } from 'vitepress'

export default {
  extends: DefaultTheme,
  setup(props: any, ctx: any) {
    // Preserve DefaultTheme setup (search, etc.)
    let parentResult: any = undefined
    try {
      const maybeSetup = (DefaultTheme as any).setup
      if (typeof maybeSetup === 'function') parentResult = maybeSetup(props, ctx)
    } catch {}

    const route = useRoute()

    // Mark JS enabled for CSS no‑js fallback
    const markJS = () => {
      try { document.documentElement.classList.add('js') } catch {}
    }

    const observe = () => {
      // Respect reduced motion — reveal instantly
      if (window.matchMedia && window.matchMedia('(prefers-reduced-motion: reduce)').matches) {
        document.querySelectorAll('.VPFeatures .item, .stat-card, .why-card, .quick-card, .featured-shot, .preview-grid img, .preview-grid .preview-card, .vp-doc h1, .vp-doc h2, .vp-doc h3, .vp-doc table, .vp-doc p, .vp-doc li, .vp-doc blockquote, .vp-doc div[class*="language-"], .vp-doc pre')
          .forEach((el) => el.classList.add('in-view'))
        return
      }
      const els = document.querySelectorAll(
        '.VPFeatures .item, .stat-card, .why-card, .quick-card, .featured-shot, .preview-grid img, .preview-grid .preview-card, .vp-doc h1, .vp-doc h2, .vp-doc h3, .vp-doc table, .vp-doc p, .vp-doc li, .vp-doc blockquote, .vp-doc div[class*="language-"], .vp-doc pre'
      )
      if (!els.length) return
      if (!('IntersectionObserver' in window)) {
        els.forEach((el) => el.classList.add('in-view'))
        return
      }
      const io = new IntersectionObserver(
        (entries) => {
          entries.forEach((e) => {
            if (e.isIntersecting) {
              e.target.classList.add('in-view')
              io.unobserve(e.target)
            }
          })
        },
        { threshold: 0.12, rootMargin: '0px 0px -8% 0px' }
      )
      els.forEach((el) => io.observe(el))
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
        if (e.key === 'Tab' && btn && img) {
          if (e.shiftKey && document.activeElement === btn) { e.preventDefault(); btn.focus() }
        }
      }
      document.addEventListener('keydown', onKey, { signal: ac() })

      // Support both plain <img> and <picture><img> with WebP
      const previews = document.querySelectorAll<HTMLImageElement>('.preview-grid img, .featured-shot img')
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
        const card = el.closest('.preview-card, .featured-shot') as HTMLElement | null
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
      nextTick(() => { observe(); setupLightbox() })
    })
    watch(() => route.path, () => nextTick(() => {
      try { if (!window.location.hash) window.scrollTo(0, 0) } catch {}
      observe(); setupLightbox()
    }))

    return parentResult
  }
}