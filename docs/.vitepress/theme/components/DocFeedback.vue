<template>
  <div class="hp-feedback" v-if="visible">
    <template v-if="!state">
      <span class="hp-feedback-label">Was this page helpful?</span>
      <div class="hp-feedback-actions">
        <button class="hp-feedback-btn" type="button" @click="vote('yes')">
          <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="M7 10v12"/><path d="M15 5.88 14 10h5.83a2 2 0 0 1 1.92 2.56l-2.33 8A2 2 0 0 1 17.5 22H4a2 2 0 0 1-2-2v-8a2 2 0 0 1 2-2h2.76a2 2 0 0 0 1.79-1.11L12 2a3.13 3.13 0 0 1 3 3.88Z"/></svg>
          <span>Yes</span>
        </button>
        <button class="hp-feedback-btn" type="button" @click="vote('no')">
          <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="M17 14V2"/><path d="M9 18.12 10 14H4.17a2 2 0 0 1-1.92-2.56l2.33-8A2 2 0 0 1 6.5 2H20a2 2 0 0 1 2 2v8a2 2 0 0 1-2 2h-2.76a2 2 0 0 0-1.79 1.11L12 22a3.13 3.13 0 0 1-3-3.88Z"/></svg>
          <span>No</span>
        </button>
      </div>
    </template>
    <p v-else-if="state === 'yes'" class="hp-feedback-done">
      <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="M20 6 9 17l-5-5"/></svg>
      Thanks — glad it helped.
    </p>
    <p v-else class="hp-feedback-done hp-feedback-sorry">
      Sorry to hear that —
      <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues" target="_blank" rel="noopener">open an issue</a>
      or
      <a href="https://discord.gg/z6yYYpcYYc" target="_blank" rel="noopener">ask on Discord</a>.
    </p>
  </div>
</template>

<script setup lang="ts">
import { onMounted, ref, watch } from 'vue'
import { useRoute } from 'vitepress'

const route = useRoute()
const state = ref<'yes' | 'no' | null>(null)
const visible = ref(false)

const keyFor = (path: string) => `hp-fb:${path}`

const load = () => {
  visible.value = !!(document.querySelector('.vp-doc'))
  let saved: string | null = null
  try { saved = localStorage.getItem(keyFor(route.path)) } catch {}
  state.value = saved === 'yes' || saved === 'no' ? saved : null
}

const vote = (v: 'yes' | 'no') => {
  state.value = v
  try { localStorage.setItem(keyFor(route.path), v) } catch {}
}

onMounted(load)
watch(() => route.path, load)
</script>
