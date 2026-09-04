/**
 * copyPage.ts — "Copy page as Markdown"
 * Converts the rendered .vp-doc article into clean GFM Markdown, in the
 * spirit of professional docs (Mintlify / Anthropic-style "Copy page").
 * Pure client-side: walks the DOM of the current article.
 */

const SKIP_SELECTOR = [
  '.header-anchor',
  'script',
  'style',
  '.vp-doc-copy-marker'
].join(', ')

function absUrl(url: string): string {
  try {
    return new URL(url, window.location.href).href
  } catch {
    return url
  }
}

/** Inline rendering: strong/em/code/links collapse into Markdown spans. */
function renderInline(node: Node): string {
  if (node.nodeType === Node.TEXT_NODE) {
    // Collapse whitespace the way HTML rendering does
    return (node.textContent || '').replace(/\s+/g, ' ')
  }
  if (node.nodeType !== Node.ELEMENT_NODE) return ''
  const el = node as HTMLElement
  if (el.matches(SKIP_SELECTOR)) return ''
  if (el.tagName === 'BR') return '\n'

  const kids = () => Array.from(el.childNodes).map(renderInline).join('')

  switch (el.tagName) {
    case 'STRONG':
    case 'B': {
      const inner = kids().trim()
      return inner ? `**${inner}**` : ''
    }
    case 'EM':
    case 'I': {
      const inner = kids().trim()
      return inner ? `*${inner}*` : ''
    }
    case 'DEL':
    case 'S': {
      const inner = kids().trim()
      return inner ? `~~${inner}~~` : ''
    }
    case 'CODE': {
      const inner = (el.textContent || '').replace(/\s+/g, ' ').trim()
      return inner ? '`' + inner + '`' : ''
    }
    case 'A': {
      const href = el.getAttribute('href') || ''
      const text = kids().replace(/\s+/g, ' ').trim()
      if (!href || href.startsWith('#')) return text
      const abs = absUrl(el.getAttribute('href') || href)
      return `[${text}](${abs})`
    }
    case 'IMG': {
      const alt = el.getAttribute('alt') || ''
      const src = absUrl(el.getAttribute('src') || '')
      return `![${alt}](${src})`
    }
    case 'BR':
      return ' '
    default: {
      // Buttons inside prose (e.g. copy buttons) contribute nothing
      if (el.tagName === 'BUTTON' || el.tagName === 'SPAN' && el.classList.contains('vp-copy')) return ''
      return kids()
    }
  }
}

function renderList(el: HTMLElement, indent: string, ordered: boolean): string {
  const lines: string[] = []
  let index = 1
  Array.from(el.children).forEach((li) => {
    if (li.tagName !== 'LI') return
    const marker = ordered ? `${index}. ` : '- '
    index++

    // Split the LI into inline content vs nested blocks (lists, paragraphs)
    let inline = ''
    const nested: string[] = []
    Array.from(li.childNodes).forEach((child) => {
      if (child.nodeType === Node.TEXT_NODE) {
        inline += renderInline(child)
        return
      }
      if (child.nodeType !== Node.ELEMENT_NODE) return
      const cel = child as HTMLElement
      if (cel.tagName === 'UL' || cel.tagName === 'OL') {
        nested.push(renderList(cel, indent + '  ', cel.tagName === 'OL'))
      } else if (cel.tagName === 'P') {
        inline += renderInline(cel)
      } else {
        nested.push(renderBlock(cel, indent + '  '))
      }
    })
    const first = `${indent}${marker}${inline.replace(/\s+/g, ' ').trim()}`
    lines.push(first)
    if (nested.length) lines.push(nested.join('\n'))
  })
  return lines.filter(Boolean).join('\n')
}

function renderTable(el: HTMLElement): string {
  const rows: string[][] = []
  el.querySelectorAll('tr').forEach((tr) => {
    const cells = Array.from(tr.children).map((cell) =>
      renderInline(cell).replace(/\|/g, '\\|').replace(/\s+/g, ' ').trim()
    )
    if (cells.length) rows.push(cells)
  })
  if (!rows.length) return ''
  const width = Math.max(...rows.map((r) => r.length))
  const pad = (r: string[]) => {
    while (r.length < width) r.push('')
    return `| ${r.join(' | ')} |`
  }
  const header = pad(rows[0])
  const divider = `| ${Array.from({ length: width }, () => '---').join(' | ')} |`
  const body = rows.slice(1).map(pad)
  return [header, divider, ...body].join('\n')
}

function codeLang(pre: HTMLElement): string {
  const wrapper = pre.closest('div[class*="language-"]') as HTMLElement | null
  if (!wrapper) return ''
  const m = wrapper.className.match(/language-([\w-]+)/)
  if (!m) return ''
  // Skip vp-demo / unknown markers
  return m[1] === 'md' ? 'markdown' : m[1]
}

/** Block rendering: headings, paragraphs, lists, tables, code, quotes. */
function renderBlock(el: HTMLElement, depth = 0): string {
  if (el.matches(SKIP_SELECTOR)) return ''
  switch (el.tagName) {
    case 'H1':
    case 'H2':
    case 'H3':
    case 'H4':
    case 'H5':
    case 'H6': {
      const level = Number(el.tagName[1])
      const text = Array.from(el.childNodes)
        .filter((n) => !(n.nodeType === 1 && (n as HTMLElement).matches(SKIP_SELECTOR)))
        .map(renderInline).join('').replace(/\s+/g, ' ').trim()
      return `${'#'.repeat(level)} ${text}`
    }
    case 'P':
      return renderInline(el).replace(/\s+/g, ' ').trim()
    case 'UL':
      return renderList(el, '', false)
    case 'OL':
      return renderList(el, '', true)
    case 'TABLE':
      return renderTable(el)
    case 'HR':
      return '---'
    case 'PRE': {
      const code = (el.querySelector('code') || el).textContent || ''
      return '```' + codeLang(el) + '\n' + code.replace(/\n$/, '') + '\n```'
    }
    case 'BLOCKQUOTE': {
      const inner = Array.from(el.children).map((c) => renderBlock(c, depth)).join('\n\n')
      return inner.split('\n').map((l) => `> ${l}`.trimEnd()).join('\n')
    }
    case 'DETAILS': {
      const summary = el.querySelector('summary')
      const sumText = summary ? renderInline(summary).replace(/\s+/g, ' ').trim() : ''
      const body: string[] = []
      Array.from(el.children).forEach((c) => {
        if (c.tagName === 'SUMMARY') return
        const out = renderBlock(c as HTMLElement, depth)
        if (out) body.push(out)
      })
      return `**${sumText}**\n\n${body.join('\n\n')}`.trim()
    }
    case 'DIV': {
      // Editorial components → sensible Markdown equivalents
      if (el.classList.contains('badge-row')) {
        const links = Array.from(el.querySelectorAll('a, span'))
          .map((n) => renderInline(n).trim()).filter(Boolean)
        return links.join(' · ')
      }
      if (el.classList.contains('custom-block')) {
        const title = el.querySelector('.custom-block-title')
        const t = title ? (title.textContent || '').toUpperCase() : 'NOTE'
        const inner = Array.from(el.children)
          .filter((c) => !(c as HTMLElement).classList.contains('custom-block-title'))
          .map((c) => renderBlock(c as HTMLElement, depth)).join('\n\n')
        return [`> **${t}**`, ...inner.split('\n').map((l) => `> ${l}`.trimEnd())].join('\n')
      }
      // Generic container: recurse over block children, ignore stray inline junk
      const out = Array.from(el.children).map((c) => renderBlock(c as HTMLElement, depth)).filter(Boolean)
      return out.join('\n\n')
    }
    default: {
      // Unknown block-ish element: try inline rendering, else recurse
      const hasBlockKids = Array.from(el.children).some((c) =>
        /^H[1-6]|P|UL|OL|TABLE|PRE|DIV|BLOCKQUOTE|DETAILS$/.test(c.tagName)
      )
      if (hasBlockKids) {
        return Array.from(el.children).map((c) => renderBlock(c as HTMLElement, depth)).filter(Boolean).join('\n\n')
      }
      return renderInline(el).replace(/\s+/g, ' ').trim()
    }
  }
}

/** Convert the main article to Markdown. skipH1 drops the article's own
 *  title (used when the caller prepends a header with the page title). */
export function docToMarkdown(root: HTMLElement, opts: { skipH1?: boolean } = {}): string {
  const parts: string[] = []
  let skippedH1 = false
  Array.from(root.children).forEach((child) => {
    if (opts.skipH1 && !skippedH1 && (child as HTMLElement).tagName === 'H1') {
      skippedH1 = true
      return
    }
    const out = renderBlock(child as HTMLElement)
    if (out.trim()) parts.push(out.trim())
  })
  return parts.join('\n\n').replace(/\n{3,}/g, '\n\n').trim() + '\n'
}

/** Clipboard with a non-secure-context fallback. */
export async function copyText(text: string): Promise<boolean> {
  try {
    if (navigator.clipboard && window.isSecureContext) {
      await navigator.clipboard.writeText(text)
      return true
    }
  } catch { /* fall through */ }
  try {
    const ta = document.createElement('textarea')
    ta.value = text
    ta.setAttribute('readonly', '')
    ta.style.position = 'fixed'
    ta.style.opacity = '0'
    ta.style.pointerEvents = 'none'
    document.body.appendChild(ta)
    ta.select()
    const ok = document.execCommand('copy')
    ta.remove()
    return ok
  } catch {
    return false
  }
}
