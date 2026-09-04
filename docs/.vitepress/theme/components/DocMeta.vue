<template>
  <div class="hp-docmeta" v-if="ready">
    <div class="hp-docmeta-left">
      <span class="hp-docmeta-item hp-docmeta-section">{{ section }}</span>
      <span class="hp-docmeta-dot" aria-hidden="true"></span>
      <span class="hp-docmeta-item">{{ readingTime }}</span>
      <template v-if="updatedLabel">
        <span class="hp-docmeta-dot" aria-hidden="true"></span>
        <span class="hp-docmeta-item hp-docmeta-updated">{{ updatedLabel }}</span>
      </template>
    </div>
    <button class="hp-docmeta-copy" type="button" @click="copyPage" :data-copied="copied ? '1' : '0'">
      <svg v-if="!copied" width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
        <rect x="9" y="9" width="12" height="12" rx="2"/><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"/>
      </svg>
      <svg v-else width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
        <path d="M20 6 9 17l-5-5"/>
      </svg>
      <span>{{ copied ? 'Copied' : 'Copy page' }}</span>
    </button>
  </div>
</template>

<script setup lang="ts">
import { nextTick, onMounted, ref, watch } from 'vue'
import { useData, useRoute } from 'vitepress'
import { docToMarkdown, copyText } from '../copyPage'

const { theme, page, site } = useData()
const route = useRoute()

const ready = ref(false)
const readingTime = ref('')
const updatedLabel = ref('')
const section = ref('Documentation')
const copied = ref(false)
let copyTimer: ReturnType<typeof setTimeout> | null = null

/** Derive the sidebar group name for the current path (e.g. "Guide").
 *  Note: route.path includes the configured base in VitePress 1.6, so it
 *  must be stripped before comparing against sidebar link config. */
const normPath = (p: string): string =>
  '/' + p.replace(/\.html$/, '').replace(/\/index$/, '/').replace(/^\/+|\/+$/g, '')

const deriveSection = (path: string): string => {
  try {
    const cfg = theme.value.sidebar as any
    if (!Array.isArray(cfg)) return 'Documentation'
    const base = String((site.value as any)?.base || '/')
    let p = path
    if (base !== '/' && p.startsWith(base)) p = p.slice(base.length)
    else {
      const baseNoSlash = base.replace(/\/$/, '')
      if (baseNoSlash && p.startsWith(baseNoSlash)) p = p.slice(baseNoSlash.length)
    }
    const clean = normPath(p)
    for (const group of cfg) {
      const items = Array.isArray(group.items) ? group.items : [group]
      for (const item of items) {
        if (!item || typeof item.link !== 'string') continue
        if (normPath(item.link) === clean) return String(group.text || 'Documentation').toUpperCase()
      }
    }
  } catch {}
  return 'Documentation'
}

const measure = () => {
  const path = route.path
  section.value = deriveSection(path)

  // Reading time from rendered article (≈220 wpm, 1 min floor)
  const doc = document.querySelector('.vp-doc') as HTMLElement | null
  if (doc) {
    const words = (doc.textContent || '').trim().split(/\s+/).filter(Boolean).length
    const mins = Math.max(1, Math.round(words / 220))
    readingTime.value = `${mins} min read`
  } else {
    readingTime.value = ''
  }

  // Last updated from git timestamp (VitePress lastUpdated: true)
  const ts = (page.value as any)?.lastUpdated
  if (ts) {
    try {
      const d = new Date(ts)
      const s = d.toLocaleDateString('en-US', { month: 'short', day: 'numeric', year: 'numeric' })
      updatedLabel.value = `Updated ${s}`
    } catch { updatedLabel.value = '' }
  } else {
    updatedLabel.value = ''
  }
  ready.value = true
}

const copyPage = async () => {
  const doc = document.querySelector('.vp-doc') as HTMLElement | null
  if (!doc) return
  const md = docToMarkdown(doc, { skipH1: true })
  const header = `# ${document.title.replace(/ — HitPaw MangaDex Manager$/, '')}\n\n> Source: ${window.location.href}\n\n`
  const ok = await copyText(header + md)
  if (ok) {
    copied.value = true
    if (copyTimer) clearTimeout(copyTimer)
    copyTimer = setTimeout(() => { copied.value = false }, 2000)
  }
}

onMounted(() => nextTick(measure))
watch(() => route.path, () => nextTick(measure))
</script>
