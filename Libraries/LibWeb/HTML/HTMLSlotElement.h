/*
 * Copyright (c) 2020, the SerenityOS developers.
 * Copyright (c) 2023, Tim Flynn <trflynn89@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Variant.h>
#include <AK/Vector.h>
#include <LibGC/Root.h>
#include <LibWeb/DOM/Slot.h>
#include <LibWeb/DOM/Slottable.h>
#include <LibWeb/HTML/HTMLElement.h>

namespace Web::HTML {

struct AssignedNodesOptions {
    bool flatten { false };
};

class HTMLSlotElement final
    : public HTMLElement
    , public DOM::Slot {
    WEB_PLATFORM_OBJECT(HTMLSlotElement, HTMLElement);
    GC_DECLARE_ALLOCATOR(HTMLSlotElement);

public:
    virtual ~HTMLSlotElement() override;

    Vector<GC::Root<DOM::Node>> assigned_nodes(AssignedNodesOptions options = {}) const;
    Vector<GC::Root<DOM::Element>> assigned_elements(AssignedNodesOptions options = {}) const;

    using SlottableHandle = Variant<GC::Root<DOM::Element>, GC::Root<DOM::Text>>;
    void assign(Vector<SlottableHandle> nodes);

    ReadonlySpan<DOM::Slottable> manually_assigned_nodes() const { return m_manually_assigned_nodes; }

private:
    HTMLSlotElement(DOM::Document&, DOM::QualifiedName);

    virtual bool is_html_slot_element() const override { return true; }

    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(JS::Cell::Visitor&) override;

    virtual void inserted() override;
    virtual void removed_from(Node* old_parent, Node& old_root) override;

    virtual void attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_) override;

    // https://html.spec.whatwg.org/multipage/scripting.html#manually-assigned-nodes
    Vector<DOM::Slottable> m_manually_assigned_nodes;
};

}

namespace Web::DOM {

template<>
inline bool Node::fast_is<HTML::HTMLSlotElement>() const { return is_html_slot_element(); }

}
