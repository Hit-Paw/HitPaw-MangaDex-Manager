<template>
  <div v-if="visible" class="hp-announce" role="region" aria-label="Announcement">
    <div class="hp-announce-inner">
      <span class="hp-announce-dot" aria-hidden="true"></span>
      <p class="hp-announce-text">
        <strong>v3.5.0 released</strong>
        <span class="hp-announce-sep" aria-hidden="true">—</span>
        <span>Show All (N), cover cache &amp; 5 offline export formats</span>
      </p>
      <a class="hp-announce-link" href="/HitPaw-MangaDex-Manager/changelog.html">
        Release notes
        <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="M5 12h14"/><path d="m12 5 7 7-7 7"/></svg>
      </a>
      <button class="hp-announce-close" type="button" aria-label="Dismiss announcement" @click="dismiss">
        <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" aria-hidden="true"><path d="M18 6 6 18"/><path d="m6 6 12 12"/></svg>
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue'

const STORAGE_KEY = 'hp-announce-v3.5.0'
const HEIGHT = '40px'
const visible = ref(false)

const setOffset = (on: boolean) => {
  try {
    if (on) document.documentElement.style.setProperty('--vp-layout-top-height', HEIGHT)
    else document.documentElement.style.removeProperty('--vp-layout-top-height')
  } catch {}
}

onMounted(() => {
  let dismissed = false
  try { dismissed = localStorage.getItem(STORAGE_KEY) === '1' } catch {}
  visible.value = !dismissed
  setOffset(visible.value)
})

onUnmounted(() => setOffset(false))

const dismiss = () => {
  visible.value = false
  setOffset(false)
  try { localStorage.setItem(STORAGE_KEY, '1') } catch {}
}
</script>