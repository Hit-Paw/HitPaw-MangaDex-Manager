import DefaultTheme from 'vitepress/theme'
import './custom.css'
import './lightbox-fix.css'
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

    // Mark JS enabled for CSS no-js fallback
    const markJS = () => {
      try { document.documentElement.classList.add('js') } catch {}
    }

    const observe = () => {
      const els = document.querySelectorAll(
        '.VPFeatures .item, .stat-card, .why-card, .quick-card, .preview-grid img, .preview-grid .preview-card, .vp-doc h1, .vp-doc h2, .vp-doc h3, .vp-doc table, .vp-doc p, .vp-doc li, .vp-doc blockquote, .vp-doc div[class*="language-"], .vp-doc pre'
      )
      if (!els.length) return
      // No IntersectionObserver fallback — reveal all immediately
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
        { threshold: 0.14, rootMargin: '0px 0px -6% 0px' }
      )
      els.forEach((el) => io.observe(el))
    }

    // Lightbox — a11y dialog, focus trap, keyboard
    let cleanupLightbox: (() => void) | null = null

    const setupLightbox = () => {
      if (cleanupLightbox) { cleanupLightbox(); cleanupLightbox = null }
      const lb = document.getElementById('lightbox') as HTMLElement | null
      if (!lb) return

      // Keep the lightbox outside VitePress content so fixed positioning is relative
      // to the actual browser viewport even when page content has transforms/animations.
      if (lb.parentElement !== document.body) {
        document.body.appendChild(lb)
      }

      // Ensure a11y attributes on lightbox container
      lb.setAttribute('role', 'dialog')
      lb.setAttribute('aria-modal', 'true')
      lb.setAttribute('aria-label', 'Image preview — press Escape to close')

      let img = lb.querySelector('img') as HTMLImageElement | null
      let caption = lb.querySelector('.lightbox-caption') as HTMLElement | null
      const closeBtn = lb.querySelector('.lightbox-close') as HTMLButtonElement | null

      // Ensure close button exists
      let btn = closeBtn
      if (!btn) {
        btn = document.createElement('button')
        btn.type = 'button'
        btn.className = 'lightbox-close'
        btn.setAttribute('aria-label', 'Close preview')
        btn.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M18 6L6 18"/><path d="M6 6l12 12"/></svg>'
        lb.appendChild(btn)
        if (!caption) {
          caption = document.createElement('div')
          caption.className = 'lightbox-caption'
          lb.appendChild(caption)
        }
      }
      if (!img) {
        img = lb.querySelector('img')
        if (!img) return
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
        // Move focus to close button for screen readers
        requestAnimationFrame(() => btn && btn.focus())
      }
      const close = () => {
        lb.classList.remove('open')
        lb.setAttribute('aria-hidden', 'true')
        document.body.style.overflow = ''
        // Restore focus
        if (lastFocus && typeof lastFocus.focus === 'function') {
          try { lastFocus.focus() } catch {}
        }
      }

      // Click on backdrop closes
      lb.addEventListener('click', (e) => { if (e.target === lb) close() }, { signal: ac() })
      if (btn) btn.addEventListener('click', close, { signal: ac() })
      if (img) img.addEventListener('click', close, { signal: ac() })

      // Keyboard — Escape, Tab trap
      const onKey = (e: KeyboardEvent) => {
        if (!lb.classList.contains('open')) return
        if (e.key === 'Escape') { e.preventDefault(); close(); return }
        if (e.key === 'Tab' && btn && img) {
          const focusables = [btn, img].filter(Boolean) as HTMLElement[]
          // Simple trap: keep focus on close button
          if (e.shiftKey && document.activeElement === btn) { e.preventDefault(); btn.focus() }
        }
      }
      document.addEventListener('keydown', onKey, { signal: ac() })

      // Enhance preview images — a11y button behavior
      const previews = document.querySelectorAll<HTMLImageElement>('.preview-grid img')
      previews.forEach((el) => {
        el.setAttribute('tabindex', '0')
        el.setAttribute('role', 'button')
        el.setAttribute('aria-label', `Enlarge preview: ${el.alt || 'screenshot'}`)
        el.style.cursor = 'zoom-in'
        const handler = () => {
          const src = (el as any).currentSrc || el.src
          open(src, el.alt || '')
        }
        el.addEventListener('click', handler, { signal: ac() })
        el.addEventListener('keydown', (e: KeyboardEvent) => {
          if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); handler() }
        }, { signal: ac() })
      })

      cleanupLightbox = () => {
        controllers.forEach((c) => c.abort())
        document.body.style.overflow = ''
      }
    }

    onMounted(() => {
      markJS()
      // Fix: refresh always jumped to bottom — disable browser scroll restoration and force top if no hash
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
      // On route change, ensure top unless hash anchor
      try { if (!window.location.hash) window.scrollTo(0, 0) } catch {}
      observe(); setupLightbox()
    }))

    return parentResult
  }
}
