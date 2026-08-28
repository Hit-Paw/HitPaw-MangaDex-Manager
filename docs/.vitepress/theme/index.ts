import DefaultTheme from 'vitepress/theme'
import './custom.css'
import { onMounted, watch, nextTick } from 'vue'
import { useRoute } from 'vitepress'

export default {
  extends: DefaultTheme,
  setup(props: any, ctx: any) {
    // Preserve DefaultTheme setup (critical for local search + other Vue logic)
    let parentResult: any = undefined
    try {
      const maybeSetup = (DefaultTheme as any).setup
      if (typeof maybeSetup === 'function') {
        parentResult = maybeSetup(props, ctx)
      }
    } catch {}
    const route = useRoute()
    const observe = () => {
      const els = document.querySelectorAll(
        '.VPFeatures .item, .stat-card, .why-card, .quick-card, .preview-grid img, .vp-doc h1, .vp-doc h2, .vp-doc h3, .vp-doc table, .vp-doc p, .vp-doc li, .vp-doc blockquote, .vp-doc div[class*="language-"], .vp-doc pre'
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
      // dedupe listeners (route changes call setupLightbox again)
      lb.replaceWith(lb.cloneNode(true))
      const freshLb = document.getElementById('lightbox') as HTMLElement | null
      if (!freshLb) return
      const freshImg = freshLb.querySelector('img') as HTMLImageElement | null
      const freshCaption = freshLb.querySelector('.lightbox-caption') as HTMLElement | null
      const freshOpen = (src: string, alt: string) => {
        if (!freshImg) return
        freshImg.src = src
        freshImg.alt = alt
        if (freshCaption) freshCaption.textContent = alt
        freshLb.classList.add('open')
        freshLb.setAttribute('aria-hidden', 'false')
        document.body.style.overflow = 'hidden'
      }
      const freshClose = () => {
        freshLb.classList.remove('open')
        freshLb.setAttribute('aria-hidden', 'true')
        document.body.style.overflow = ''
      }
      freshLb.addEventListener('click', (e) => {
        if (e.target === freshLb) freshClose()
      })
      document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape' && freshLb.classList.contains('open')) freshClose()
      })
      document.querySelectorAll<HTMLImageElement>('.preview-grid img').forEach((el) => {
        el.style.cursor = 'zoom-in'
        el.addEventListener('click', () => {
          const src = (el as any).currentSrc || el.src
          freshOpen(src, el.alt || '')
        })
      })
      if (freshImg) {
        freshImg.addEventListener('click', freshClose)
      }
    }
    onMounted(() => {
      nextTick(() => {
        observe()
        setupLightbox()
      })
    })
    watch(
      () => route.path,
      () =>
        nextTick(() => {
          observe()
          setupLightbox()
        })
    )
    return parentResult
  }
}
