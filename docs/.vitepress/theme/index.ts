import DefaultTheme from 'vitepress/theme'
import './custom.css'
import { onMounted, watch, nextTick } from 'vue'
import { useRoute } from 'vitepress'

export default {
  extends: DefaultTheme,
  setup() {
    const route = useRoute()
    const observe = () => {
      const els = document.querySelectorAll(
        '.VPFeatures .item, .stat-card, .why-card, .quick-card, .preview-grid img, .vp-doc h1, .vp-doc h2, .vp-doc h3, .vp-doc table, .vp-doc p, .vp-doc li, .vp-doc blockquote, .vp-doc div[class*=\"language-\"], .vp-doc pre'
      )
      const io = new IntersectionObserver(
        (entries) => {
          entries.forEach((e) => {
            if (e.isIntersecting) {
              e.target.classList.add('in-view')
              io.unobserve(e.target)
            }
          })
        },
        { threshold: 0.15 }
      )
      els.forEach((el) => io.observe(el))
    }
    const setupLightbox = () => {
      const lb = document.getElementById('lightbox') as HTMLElement | null
      if (!lb) return
      const img = lb.querySelector('img') as HTMLImageElement | null
      const caption = lb.querySelector('.lightbox-caption') as HTMLElement | null
      const open = (src: string, alt: string) => {
        if (!img) return
        img.src = src
        img.alt = alt
        if (caption) caption.textContent = alt
        lb.classList.add('open')
        lb.setAttribute('aria-hidden', 'false')
        document.body.style.overflow = 'hidden'
      }
      const close = () => {
        lb.classList.remove('open')
        lb.setAttribute('aria-hidden', 'true')
        document.body.style.overflow = ''
      }
      lb.addEventListener('click', (e) => {
        if (e.target === lb) close()
      })
      document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape' && lb.classList.contains('open')) close()
      })
      document.querySelectorAll<HTMLImageElement>('.preview-grid img').forEach((el) => {
        el.style.cursor = 'zoom-in'
        // use currentSrc (resolved with base) — data-full was missing base (/HitPaw-MangaDex-Manager/) and 404'd
        el.addEventListener('click', () => {
          const src = (el as any).currentSrc || el.src
          open(src, el.alt || '')
        })
      })
      // close on image click as well
      if (img) {
        img.addEventListener('click', () => {
          const lbEl = document.getElementById('lightbox')
          if (lbEl?.classList.contains('open')) {
            lbEl.classList.remove('open')
            lbEl.setAttribute('aria-hidden', 'true')
            document.body.style.overflow = ''
          }
        })
      }
    }
    onMounted(() => {
      nextTick(() => {
        observe()
        setupLightbox()
      })
    })
    watch(() => route.path, () =>
      nextTick(() => {
        observe()
        setupLightbox()
      })
    )
  }
}
