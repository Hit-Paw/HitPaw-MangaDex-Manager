import DefaultTheme from 'vitepress/theme'
import './custom.css'
import { onMounted, watch, nextTick } from 'vue'
import { useRoute } from 'vitepress'

export default {
  extends: DefaultTheme,
  setup() {
    const route = useRoute()
    const observe = () => {
      const els = document.querySelectorAll('.VPFeatures .item, .vp-doc img, .stat-card')
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
    onMounted(() => {
      nextTick(observe)
    })
    watch(() => route.path, () => nextTick(observe))
  }
}
